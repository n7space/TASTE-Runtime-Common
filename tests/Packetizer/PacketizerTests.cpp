#include "CppUTest/TestHarness.h"

#include <cstring>

extern "C"
{
#include "Packetizer.h"
#include "SpacePacket.h"
}

TEST_GROUP(Packetizer)
{
    static constexpr size_t dataSize = 514;
    static constexpr size_t packetSize =
            SPACE_PACKET_PRIMARY_HEADER_SIZE_BYTES + dataSize + SPACE_PACKET_ERROR_CONTROL_SIZE_BYTES;
    uint8_t packetData[packetSize];

    void setup() { memset(packetData, 0, packetSize); }
};

TEST(Packetizer, PacketizeTelemetry)
{
    const uint16_t destination = 682;
    const uint16_t sequenceCount = 792;

    Packetizer packetizer = { .packetSequenceCount = sequenceCount };

    auto packetSize = Packetizer_packetize(&packetizer,
                                           Packetizer_PacketType_Telemetry,
                                           0,
                                           destination,
                                           packetData,
                                           SPACE_PACKET_PRIMARY_HEADER_SIZE_BYTES,
                                           dataSize);

    CHECK_EQUAL(0b01010000, packetData[0]);
    CHECK_EQUAL(0b10101010, packetData[1]);
    CHECK_EQUAL(0b00001111, packetData[2]);
    CHECK_EQUAL(0b00011000, packetData[3]);
    CHECK_EQUAL(0b00000010, packetData[4]);
    CHECK_EQUAL(0b00000001, packetData[5]);
};

TEST(Packetizer, PacketizeTelecommand)
{
    const uint16_t destination = 1365;
    const uint16_t sequenceCount = 793;

    Packetizer packetizer = { .packetSequenceCount = sequenceCount };

    auto packetSize = Packetizer_packetize(&packetizer,
                                           Packetizer_PacketType_Telecommand,
                                           0,
                                           destination,
                                           packetData,
                                           SPACE_PACKET_PRIMARY_HEADER_SIZE_BYTES,
                                           dataSize);
};

TEST(Packetizer, DepacketizeTelemetry)
{
    Packetizer packetizer = { .packetSequenceCount = 8 };

    packetData[0] = 0b10110000;
    packetData[1] = 0b00000101;
    packetData[4] = 0b00000010;
    packetData[5] = 0b00000001;

    uint16_t receivedDestination = 0;
    uint32_t receivedDataOffset = 0;
    uint32_t receivedDataSize = 0;
    uint32_t receivedErrorCode = 0;

    auto success = Packetizer_depacketize(&packetizer,
                                          Packetizer_PacketType_Telemetry,
                                          packetData,
                                          packetSize,
                                          NULL,
                                          &receivedDestination,
                                          &receivedDataOffset,
                                          &receivedDataSize,
                                          &receivedErrorCode);

    CHECK_EQUAL(0, receivedErrorCode);
    CHECK(success);

    CHECK_EQUAL(0x0505, receivedDestination);
    CHECK_EQUAL(SPACE_PACKET_PRIMARY_HEADER_SIZE_BYTES, receivedDataOffset);
    CHECK_EQUAL(dataSize - 1, receivedDataSize);
};

TEST(Packetizer, DepacketizeTelecommand)
{
    Packetizer packetizer = { .packetSequenceCount = 7 };

    packetData[0] = 0b11011000;
    packetData[1] = 0b00000110;
    packetData[4] = 0b00000010;
    packetData[5] = 0b00000001;

    uint16_t receivedDestination = 0;
    uint32_t receivedDataOffset = 0;
    uint32_t receivedDataSize = 0;
    uint32_t receivedErrorCode = 0;

    auto success = Packetizer_depacketize(&packetizer,
                                          Packetizer_PacketType_Telecommand,
                                          packetData,
                                          packetSize,
                                          NULL,
                                          &receivedDestination,
                                          &receivedDataOffset,
                                          &receivedDataSize,
                                          &receivedErrorCode);

    CHECK_EQUAL(0, receivedErrorCode);
    CHECK(success);

    CHECK_EQUAL(0x606, receivedDestination);
    CHECK_EQUAL(SPACE_PACKET_PRIMARY_HEADER_SIZE_BYTES, receivedDataOffset);
    CHECK_EQUAL(dataSize - 1, receivedDataSize);
};
