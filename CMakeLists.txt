#
# Initial CMake and project setup
#

cmake_minimum_required(VERSION 3.0.2)

set(PROJECT_NAME tritium)
project(${PROJECT_NAME})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/build-data)

include(ExternalProject)

#
# Setup GCC flags
#

set(CMAKE_C_FLAGS "-O3")

#
# Find/install dependencies
#

find_package(PortAudio)
if (${PORTAUDIO_FOUND})
	include_directories(${PORTAUDIO_INCLUDE_DIRS})
else ()
	ExternalProject_Add(
		PortAudio
        GIT_REPOSITORY      "https://github.com/syb0rg/PortAudio2.git"
		SOURCE_DIR          ${CMAKE_BINARY_DIR}/lib/PortAudio
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
	set(PORTAUDIO_SOURCE_DIR ${SOURCE_DIR})
	set(PORTAUDIO_BINARY_DIR ${BINARY_DIR})
	if (APPLE)
		set(PORTAUDIO_LIBRARIES ${PORTAUDIO_SOURCE_DIR}/libportaudio.dylib)
	elseif (UNIX)
		set(PORTAUDIO_LIBRARIES ${PORTAUDIO_SOURCE_DIR}/libportaudio.so)
	endif ()
	include_directories(${PORTAUDIO_SOURCE_DIR}/include)
	set(DEPENDENCIES ${DEPENDENCIES} PortAudio)
endif ()

if (WIN32)
	SET(WIN32_LIBS ${WIN32_LIBS} setupapi winmm)
else ()
	SET(UNIX_LIBS ${UNIX_LIBS} m pthread)
endif ()
SET(LIBS ${LIBS} ${PORTAUDIO_LIBRARIES} ${UNIX_LIBS} ${WIN32_LIBS})

#
# Final project settings
#

include_directories(include)
add_subdirectory(src)
add_subdirectory(lang)
add_subdirectory(main)
#add_subdirectory(tests EXCLUDE_FROM_ALL)

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
target_compile_features(${PROJECT_NAME} PRIVATE c_restrict)
target_link_libraries(${PROJECT_NAME} ${LIBS})

if (DEPENDENCIES)
	message(STATUS "Any missing dependencies will be downloaded upon running \"make\"")
	add_dependencies(${PROJECT_NAME} ${DEPENDENCIES})
endif ()

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)
install(DIRECTORY include/
        DESTINATION include/${PROJECT_NAME}
)
