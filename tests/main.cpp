#include "CppUTest/CommandLineTestRunner.h"

IMPORT_TEST_GROUP(Packetizer);
IMPORT_TEST_GROUP(Escaper);

int
main(int argc, char** argv)
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
