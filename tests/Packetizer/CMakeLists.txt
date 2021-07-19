project(PacketizerTests VERSION 1.0.0 LANGUAGES C CXX)

add_executable(PacketizerTests)
target_sources(PacketizerTests
    PRIVATE     main.cpp
                PacketizerTests.cpp)
target_link_libraries(PacketizerTests
    PRIVATE     common_build_options
                TASTE::Packetizer
                CppUTest
                atomic
                m)
target_compile_definitions(PacketizerTests
    PRIVATE     CPPUTEST_MEM_LEAK_DETECTION_DISABLED)

set_target_properties(PacketizerTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TESTS_OUTPUT_PATH})

add_format_target(PacketizerTests)

add_test(PacketizerTests ${TESTS_OUTPUT_PATH}/PacketizerTests)
