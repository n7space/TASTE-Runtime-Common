project(RunAllTests VERSION 1.0.0 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)

add_executable(RunAllTests)
target_sources(RunAllTests
    PRIVATE     main.cc)
set_source_files_properties(main.cc PROPERTIES SKIP_LINTING ON)

target_link_libraries(RunAllTests
    PRIVATE     common_build_options
                CppUTest
                PacketizerTests
                EscaperTests
				BrokerTests)

set_target_properties(RunAllTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TESTS_OUTPUT_PATH})

add_format_target(RunAllTests)

add_test(RunAllTests ${TESTS_OUTPUT_PATH}/RunAllTests)

add_subdirectory(Packetizer)
add_subdirectory(Escaper)
add_subdirectory(Broker)
