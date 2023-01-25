project(BrokerTests VERSION 1.0.0 LANGUAGES CXX)

add_library(BrokerTests SHARED)
target_sources(BrokerTests
    PRIVATE     BrokerTests.cc)
target_link_libraries(BrokerTests
    PRIVATE     TASTE::Broker
                TASTE::RuntimeMocks
    PUBLIC      CppUTest
                CppUTestExt)
target_compile_definitions(BrokerTests
    PUBLIC      CPPUTEST_MEM_LEAK_DETECTION_DISABLED)

set_target_properties(BrokerTests PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${TESTS_OUTPUT_PATH})

add_format_target(BrokerTests)
