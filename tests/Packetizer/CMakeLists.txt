project(PacketizerTests VERSION 1.0.0 LANGUAGES CXX)

add_library(PacketizerTests SHARED)
target_sources(PacketizerTests
    PRIVATE     PacketizerTests.cc)
target_link_libraries(PacketizerTests
    PRIVATE     common_build_options
    PUBLIC      CppUTest
                TASTE::Packetizer)
target_compile_definitions(PacketizerTests
    PUBLIC      CPPUTEST_MEM_LEAK_DETECTION_DISABLED)

set_target_properties(PacketizerTests PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${TESTS_OUTPUT_PATH})

add_format_target(PacketizerTests)
