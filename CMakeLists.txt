cmake_minimum_required(VERSION 3.0.2)
include(ExternalProject)
set(PROJECT_NAME tritium)
project(${PROJECT_NAME})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/library-build)
set(CMAKE_C_FLAGS "-std=gnu11 -O3")

FIND_PACKAGE(PortAudio)
if (${PORTAUDIO_FOUND})
	INCLUDE_DIRECTORIES(${PORTAUDIO_INCLUDE_DIRS})
else ()
	ExternalProject_Add(
		PortAudio
        	GIT_REPOSITORY      "https://github.com/syb0rg/PortAudio2.git"
		SOURCE_DIR          ${CMAKE_CURRENT_SOURCE_DIR}/lib/PortAudio
		UPDATE_COMMAND      ""
		INSTALL_COMMAND     ""
        	BUILD_IN_SOURCE     ON
		LOG_DOWNLOAD        ON
		LOG_UPDATE          ON
		LOG_CONFIGURE       ON
		LOG_BUILD           ON
		LOG_TEST            ON
		LOG_INSTALL         ON
	)
	ExternalProject_Get_Property(PortAudio SOURCE_DIR)
	ExternalProject_Get_Property(PortAudio BINARY_DIR)
	SET(PORTAUDIO_SOURCE_DIR ${SOURCE_DIR})
	SET(PORTAUDIO_BINARY_DIR ${BINARY_DIR})
	SET(PORTAUDIO_LIBRARIES ${PORTAUDIO_BINARY_DIR}/lib/.libs/libportaudio.dylib)
	INCLUDE_DIRECTORIES(${PORTAUDIO_SOURCE_DIR}/include)
	SET(DEPENDENCIES ${DEPENDENCIES} PortAudio)
endif ()
SET(LIBS ${LIBS} ${PORTAUDIO_LIBRARIES})
LINK_DIRECTORIES(${PORTAUDIO_LIBRARIES})

INCLUDE_DIRECTORIES(include)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(lang)

ADD_LIBRARY(${PROJECT_NAME} STATIC
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

if (DEPENDENCIES)
	MESSAGE (STATUS "Any missing dependencies will be downloaded upon running \"make\"")
	ADD_DEPENDENCIES(${PROJECT_NAME} ${DEPENDENCIES})
endif ()

INSTALL(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	)
INSTALL(DIRECTORY include/
	DESTINATION include/${PROJECT_NAME}
	)