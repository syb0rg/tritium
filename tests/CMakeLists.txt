set(TESTS   asciiS2U_main
            asciiU2S_main
            bin2ascii_main
            by_word_main
            combine_waves_main
            compare_wave_main
            hrg_test_main
            kal_test_main
            lex_lookup_main
            lex_test_main
            lpc_resynth_main
            lpc_test2_main
            lpc_test_main
            multi_thread_main
            nums_test_main
            play_client_main
            play_server_main
            play_sync_main
            play_wave_main
            record_in_noise_main
            record_wave_main
            regex_test_main
            rfc_main
            token_test_main
            utt_test_main
)

enable_testing()
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS ${TESTS})
set(CMAKE_CTEST_COMMAND ctest -V)

foreach (test ${TESTS})
    add_executable(${test} ${test})
    target_link_libraries(${test} sndfile)
    set_target_properties(${test}
                        PROPERTIES
                        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    add_test(NAME ${test}
            COMMAND ${test} all
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    add_dependencies(check ${test})
endforeach ()