project(RunAllTests VERSION 1.0.0 LANGUAGES CXX)

add_executable(RunAllTests)
target_sources(RunAllTests
    PRIVATE     main.cpp)
target_link_libraries(RunAllTests
    PRIVATE     common_build_options
                CppUTest
                PacketizerTests)

set_target_properties(RunAllTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TESTS_OUTPUT_PATH})

add_format_target(RunAllTests)

add_test(RunAllTests ${TESTS_OUTPUT_PATH}/RunAllTests)

add_subdirectory(Packetizer)
