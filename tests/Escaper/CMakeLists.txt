project(EscaperTests VERSION 1.0.0 LANGUAGES CXX)

add_library(EscaperTests SHARED)
target_sources(EscaperTests
    PRIVATE     EscaperTests.cc)
target_link_libraries(EscaperTests
    PRIVATE     TASTE::Escaper
    PUBLIC      CppUTest)
target_compile_definitions(EscaperTests
    PUBLIC      CPPUTEST_MEM_LEAK_DETECTION_DISABLED)

set_target_properties(EscaperTests PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${TESTS_OUTPUT_PATH})

add_format_target(EscaperTests)
