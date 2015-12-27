#ifndef _CST_SOCKET_H__
#define _CST_SOCKET_H__

int cst_socket_open(const char *host, int port);
int cst_socket_close(int socket);

int cst_socket_server(const char *name, int port,
		      int (process_client)(int name, int fd));

#endif
