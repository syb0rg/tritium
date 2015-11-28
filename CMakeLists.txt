cmake_minimum_required(VERSION 3.0.2)
include(ExternalProject)
set(PROJECT_NAME tritium)
project(${PROJECT_NAME})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/library-build)
set(CMAKE_C_FLAGS "-std=gnu11 -O3")

# TODO: dependency if not installed locally
FIND_PACKAGE(PortAudio)
INCLUDE_DIRECTORIES(${PORTAUDIO_INCLUDE_DIRS})
LINK_DIRECTORIES(${PORTAUDIO_LIBRARIES})

include_directories(include)
add_subdirectory(src)
add_subdirectory(lang)

add_library(${PROJECT_NAME} STATIC
	$<TARGET_OBJECTS:audio> 
	$<TARGET_OBJECTS:util>
	$<TARGET_OBJECTS:regex>
	$<TARGET_OBJECTS:hrg>
	$<TARGET_OBJECTS:stats>
	$<TARGET_OBJECTS:speech>
	$<TARGET_OBJECTS:lexicon>
	$<TARGET_OBJECTS:synth>
	$<TARGET_OBJECTS:wavesynth>
	$<TARGET_OBJECTS:cg>
	$<TARGET_OBJECTS:cmulex>
	$<TARGET_OBJECTS:usenglish>
	$<TARGET_OBJECTS:cmu_us_kal>
	$<TARGET_OBJECTS:cmu_time_awb>
	$<TARGET_OBJECTS:cmu_us_kal16>
	$<TARGET_OBJECTS:cmu_us_awb>
	$<TARGET_OBJECTS:cmu_us_rms>
	$<TARGET_OBJECTS:cmu_us_slt>
	)
install(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	)
install(DIRECTORY include/
	DESTINATION include/${PROJECT_NAME}
	)
#add_dependencies(${PROJECT_NAME} ${DEPENDENCIES})