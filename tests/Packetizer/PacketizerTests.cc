#include "CppUTest/TestHarness.h"

#include <cstring>

extern "C"
{
#include "Crc16.h"
#include "Packetizer.h"
#include "PacketizerInternal.h"
#include "SpacePacketInternal.h"
}

TEST_GROUP(Packetizer)
{
    static constexpr size_t dataSize = 514;
    static constexpr size_t packetSize = SPACE_PACKET_PRIMARY_HEADER_SIZE + dataSize + SPACE_PACKET_ERROR_CONTROL_SIZE;
    uint8_t packetData[packetSize];

    void setup() { memset(packetData, 0, packetSize); }
};

TEST(Packetizer, PacketizeTelemetry)
{
    const uint16_t destination = 682;
    const uint16_t sequenceCount = 792;

    Packetizer packetizer{ sequenceCount };

    auto resultSize = Packetizer_packetize(&packetizer,
                                           Packetizer_PacketType_Telemetry,
                                           0,
                                           destination,
                                           packetData,
                                           SPACE_PACKET_PRIMARY_HEADER_SIZE,
                                           dataSize);

    CHECK_EQUAL(packetSize, resultSize);
    CHECK_EQUAL(0b01010000, packetData[0]);
    CHECK_EQUAL(0b10101010, packetData[1]);
    CHECK_EQUAL(0b00001111, packetData[2]);
    CHECK_EQUAL(0b00011000, packetData[3]);
    CHECK_EQUAL(0b00000010, packetData[4]);
    CHECK_EQUAL(0b00000001, packetData[5]);

    uint16_t expectedCrc = calculateCrc16(packetData, packetSize - SPACE_PACKET_ERROR_CONTROL_SIZE);

    CHECK_EQUAL((expectedCrc >> 8u) & 0xFF, packetData[packetSize - 2]);
    CHECK_EQUAL(expectedCrc & 0xFF, packetData[packetSize - 1]);
}

TEST(Packetizer, PacketizeTelecommand)
{
    const uint16_t destination = 1365;
    const uint16_t sequenceCount = 793;

    Packetizer packetizer = { sequenceCount };

    auto resultSize = Packetizer_packetize(&packetizer,
                                           Packetizer_PacketType_Telecommand,
                                           0,
                                           destination,
                                           packetData,
                                           SPACE_PACKET_PRIMARY_HEADER_SIZE,
                                           dataSize);

    CHECK_EQUAL(packetSize, resultSize);
    CHECK_EQUAL(0b10111000, packetData[0]);
    CHECK_EQUAL(0b01010101, packetData[1]);
    CHECK_EQUAL(0b00001111, packetData[2]);
    CHECK_EQUAL(0b00011001, packetData[3]);
    CHECK_EQUAL(0b00000010, packetData[4]);
    CHECK_EQUAL(0b00000001, packetData[5]);

    uint16_t expectedCrc = calculateCrc16(packetData, packetSize - SPACE_PACKET_ERROR_CONTROL_SIZE);

    CHECK_EQUAL((expectedCrc >> 8u) & 0xFF, packetData[packetSize - 2]);
    CHECK_EQUAL(expectedCrc & 0xFF, packetData[packetSize - 1]);
}

TEST(Packetizer, DepacketizeTelemetry)
{
    Packetizer packetizer = { 8 };

    packetData[0] = 0b10110000;
    packetData[1] = 0b00000101;
    packetData[4] = 0b00000010;
    packetData[5] = 0b00000001;

    uint16_t crc = calculateCrc16(packetData, packetSize - SPACE_PACKET_ERROR_CONTROL_SIZE);
    packetData[packetSize - 2] = (crc >> 8u) & 0xFF;
    packetData[packetSize - 1] = crc & 0xFF;

    uint16_t receivedDestination = 0;
    size_t receivedDataOffset = 0;
    size_t receivedDataSize = 0;
    int32_t receivedErrorCode = 0;

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
    CHECK_EQUAL(SPACE_PACKET_PRIMARY_HEADER_SIZE, receivedDataOffset);
    CHECK_EQUAL(dataSize, receivedDataSize);
}

TEST(Packetizer, DepacketizeTelecommand)
{
    Packetizer packetizer = { 7 };

    packetData[0] = 0b11011000;
    packetData[1] = 0b00000110;
    packetData[4] = 0b00000010;
    packetData[5] = 0b00000001;

    uint16_t crc = calculateCrc16(packetData, packetSize - SPACE_PACKET_ERROR_CONTROL_SIZE);
    packetData[packetSize - 2] = (crc >> 8u) & 0xFF;
    packetData[packetSize - 1] = crc & 0xFF;

    uint16_t receivedDestination = 0;
    size_t receivedDataOffset = 0;
    size_t receivedDataSize = 0;
    int32_t receivedErrorCode = 0;

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
    CHECK_EQUAL(SPACE_PACKET_PRIMARY_HEADER_SIZE, receivedDataOffset);
    CHECK_EQUAL(dataSize, receivedDataSize);
}

TEST_GROUP(PacketizerInternal)
{
    uint8_t packetData[SPACE_PACKET_PRIMARY_HEADER_SIZE];

    void setup() { memset(packetData, 0, SPACE_PACKET_PRIMARY_HEADER_SIZE); }
};

TEST(PacketizerInternal, ApidMin)
{
    writePacketId(packetData, Packetizer_PacketType_Telecommand, 0);

    CHECK_EQUAL(0b00011000, packetData[0]);
    CHECK_EQUAL(0b00000000, packetData[1]);
}

TEST(PacketizerInternal, ApidStandard)
{
    writePacketId(packetData, Packetizer_PacketType_Telecommand, 685);

    CHECK_EQUAL(0b01011000, packetData[0]);
    CHECK_EQUAL(0b10101101, packetData[1]);
}

TEST(PacketizerInternal, ApidMax)
{
    writePacketId(packetData, Packetizer_PacketType_Telecommand, SPACE_PACKET_MAX_APID);
    CHECK_EQUAL(0b11111000, packetData[0]);
    CHECK_EQUAL(0b11111111, packetData[1]);
}

TEST(PacketizerInternal, SequenceCounterMin)
{
    Packetizer packetizer = { 0 };
    writePacketSequenceControl(packetData, &packetizer);

    CHECK_EQUAL(0b00000011, packetData[2]);
    CHECK_EQUAL(0b00000000, packetData[3]);
}

TEST(PacketizerInternal, SequenceCounterStandard)
{
    Packetizer packetizer = { 718 };
    writePacketSequenceControl(packetData, &packetizer);

    CHECK_EQUAL(0b00001011, packetData[2]);
    CHECK_EQUAL(0b11001110, packetData[3]);
}

TEST(PacketizerInternal, SequenceCounterMax)
{
    Packetizer packetizer = { SPACE_PACKET_MAX_PACKET_SEQUENCE_COUNT };
    writePacketSequenceControl(packetData, &packetizer);

    CHECK_EQUAL(0b11111111, packetData[2]);
    CHECK_EQUAL(0b11111111, packetData[3]);
}

TEST(PacketizerInternal, DataSizeMin)
{
    writePacketDataLength(packetData, 1);

    CHECK_EQUAL(0b00000000, packetData[4]);
    CHECK_EQUAL(0b00000000, packetData[5]);
}

TEST(PacketizerInternal, DataSizeStandard)
{
    writePacketDataLength(packetData, 43606);

    CHECK_EQUAL(0b10101010, packetData[4]);
    CHECK_EQUAL(0b01010101, packetData[5]);
}

TEST(PacketizerInternal, DataSizeMax)
{
    writePacketDataLength(packetData, SPACE_PACKET_MAX_PACKET_DATA_SIZE);

    CHECK_EQUAL(0b11111111, packetData[4]);
    CHECK_EQUAL(0b11111111, packetData[5]);
}
