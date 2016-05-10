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
		SVN_REPOSITORY      "https://subversion.assembla.com/svn/portaudio/portaudio/trunk/"
		SVN_TRUST_CERT      1
		SOURCE_DIR          ${CMAKE_CURRENT_SOURCE_DIR}/lib/PortAudio
		CONFIGURE_COMMAND   ${CMAKE_CURRENT_SOURCE_DIR}/lib/PortAudio/configure --prefix=<INSTALL_DIR>
		BUILD_COMMAND       make
		UPDATE_COMMAND      ""
		INSTALL_COMMAND     make install
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

if (DEPENDENCIES)
	add_dependencies(${PROJECT_NAME} ${DEPENDENCIES})
endif ()

install(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	)
install(DIRECTORY include/
	DESTINATION include/${PROJECT_NAME}
	)