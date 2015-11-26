cmake_minimum_required(VERSION 2.8)
project(Tritium_Source)

add_subdirectory(audio)
add_subdirectory(utils)
add_subdirectory(regex)
add_subdirectory(hrg)
add_subdirectory(stats)
add_subdirectory(speech)
add_subdirectory(lexicon)
add_subdirectory(synth)
add_subdirectory(wavesynth)
add_subdirectory(cg)

#add_library(${PROJECT_NAME} INTERFACE)

#target_link_libraries(${PROJECT_NAME} INTERFACE audio)