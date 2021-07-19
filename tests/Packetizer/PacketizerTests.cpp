#include "CppUTest/TestHarness.h"

extern "C"
{
#include "Packetizer.h"
}

TEST_GROUP(Packetizer){

};

TEST(Packetizer, testOk)
{
    CHECK(true);
}

TEST(Packetizer, testFail)
{
    CHECK(false);
}
