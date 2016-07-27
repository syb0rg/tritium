set(PROGRAMS 	compile_regexes
				t2p_main
				tritium_main
				tritium_time_main
	)
	
foreach (program ${PROGRAMS})
	add_executable(${program} ${program} tritium_voice_list.c)
	target_link_libraries(${program} ${PROJECT_NAME} ${PORTAUDIO_LIBRARIES})
	set_target_properties (${program} 	
							PROPERTIES
							RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
							)
endforeach ()