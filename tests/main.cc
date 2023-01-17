#include "CppUTest/CommandLineTestRunner.h"

IMPORT_TEST_GROUP(Packetizer);
IMPORT_TEST_GROUP(Escaper);
IMPORT_TEST_GROUP(Broker);

int
main(int argc, char** argv)
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
