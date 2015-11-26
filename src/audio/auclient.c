#include <stdlib.h>
#include "cst_file.h"
#include "cst_socket.h"
#include "cst_string.h"
#include "cst_wave.h"
#include "cst_audio.h"

#ifndef CST_NO_SOCKETS

#include <unistd.h>

int play_wave_client(cst_wave *w,const char *servername,int port,
		     const char *encoding)
{
    int audiofd,q,i,n,r;
    int sample_width;
    unsigned char bytes[CST_AUDIOBUFFSIZE];
    short shorts[CST_AUDIOBUFFSIZE];
    snd_header header;

    if (!w)
	return CST_ERROR_FORMAT;

    if ((audiofd = cst_socket_open(servername,port)) == 0)
	return CST_ERROR_FORMAT;

    header.magic = (unsigned int)0x2e736e64;
    header.hdr_size = sizeof(header);
    if (streq(encoding,"ulaw"))
    {
	sample_width = 1;
	header.encoding = 1; /* ulaw */
    }
    else if (streq(encoding,"uchar"))
    {
	sample_width = 1;
	header.encoding = 2; /* unsigned char */
    }
    else 
    {
	sample_width = 2;
	header.encoding = 3; /* short */
    }
    header.data_size = sample_width * w->num_samples * w->num_channels;
    header.sample_rate = w->sample_rate;
    header.channels = w->num_channels;
    if (CST_LITTLE_ENDIAN)
    {   /* If I'm intel etc swap things, so "network byte order" */
	header.magic = SWAPINT(header.magic);
	header.hdr_size = SWAPINT(header.hdr_size);
	header.data_size = SWAPINT(header.data_size);
	header.encoding = SWAPINT(header.encoding);
	header.sample_rate = SWAPINT(header.sample_rate);
	header.channels = SWAPINT(header.channels);
    }

    if (write(audiofd, &header, sizeof(header)) != sizeof(header))
    {
	cst_errmsg("auclinet: failed to write header to server\n");
	return CST_ERROR_FORMAT;
    }

    for (i=0; i < w->num_samples; i += r)
    {
	if (w->num_samples > i+CST_AUDIOBUFFSIZE)
	    n = CST_AUDIOBUFFSIZE;
	else
	    n = w->num_samples-i;
	if (streq(encoding,"ulaw"))
	{
	    for (q=0; q<n; q++)
		bytes[q] = cst_short_to_ulaw(w->samples[i+q]);
	    r = write(audiofd,bytes,n);
	}
	else 
	{
	    for (q=0; q<n; q++)
		if (CST_LITTLE_ENDIAN)
		    shorts[q] = SWAPSHORT(w->samples[i+q]);
		else
		    shorts[q] = w->samples[i+q];
	    r = write(audiofd,shorts,n*2);
	    r /= 2;
	}
	if (r <= 0)
	    cst_errmsg("failed to write %d samples\n",n);
    }

    cst_socket_close(audiofd);

    return CST_OK_FORMAT;
}

#endif
