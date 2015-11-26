set(SPEECH_SOURCE_FILES
	cst_wave.c 
	cst_wave_io.c 
	cst_track.c 
	cst_track_io.c
	cst_wave_utils.c 
	cst_lpcres.c 
	rateconv.c
)
add_library(speech OBJECT ${SPEECH_SOURCE_FILES})