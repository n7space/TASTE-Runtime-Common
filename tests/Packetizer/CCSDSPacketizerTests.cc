#include "CppUTest/TestHarness.h"

#include <cstring>

extern "C"
{
#include "IsoChecksum.h"
#include "CCSDSPacketizer.h"
#include "PacketizerInternal.h"
#include "SpacePacketInternal.h"
}

TEST_GROUP(CCSDSPacketizer)
{
    static constexpr size_t dataSize = 514;
    static constexpr size_t packetSize =
            CCSDS_SPACE_PACKET_PRIMARY_HEADER_SIZE + dataSize + SPACE_PACKET_ERROR_CONTROL_SIZE;
    uint8_t packetData[packetSize];

    void setup() { memset(packetData, 0, packetSize); }
};

TEST(CCSDSPacketizer, CCSDSPacketizeTelemetry)
{
    const uint16_t destination = 682;
    const uint16_t sequenceCount = 792;

    Packetizer packetizer{ sequenceCount };

    auto resultSize = CCSDS_Packetizer_packetize(&packetizer,
                                                 Packetizer_PacketType_Telemetry,
                                                 0,
                                                 destination,
                                                 packetData,
                                                 CCSDS_SPACE_PACKET_PRIMARY_HEADER_SIZE,
                                                 dataSize);

    CHECK_EQUAL(packetSize, resultSize);
    CHECK_EQUAL(0b00001010, packetData[0]);
    CHECK_EQUAL(0b10101010, packetData[1]);
    CHECK_EQUAL(0b11000011, packetData[2]);
    CHECK_EQUAL(0b00011000, packetData[3]);
    CHECK_EQUAL(0b00000010, packetData[4]);
    CHECK_EQUAL(0b00000011, packetData[5]);

    uint16_t expectedCrc = IsoChecksum_calculate(packetData, packetSize - SPACE_PACKET_ERROR_CONTROL_SIZE);

    CHECK_EQUAL((expectedCrc >> 8u) & 0xFF, packetData[packetSize - 2]);
    CHECK_EQUAL(expectedCrc & 0xFF, packetData[packetSize - 1]);
}

TEST(CCSDSPacketizer, CCSDSPacketizeTelecommand)
{
    const uint16_t destination = 1365;
    const uint16_t sequenceCount = 793;

    Packetizer packetizer = { sequenceCount };

    auto resultSize = CCSDS_Packetizer_packetize(&packetizer,
                                                 Packetizer_PacketType_Telecommand,
                                                 0,
                                                 destination,
                                                 packetData,
                                                 CCSDS_SPACE_PACKET_PRIMARY_HEADER_SIZE,
                                                 dataSize);

    CHECK_EQUAL(packetSize, resultSize);
    CHECK_EQUAL(0b00011101, packetData[0]);
    CHECK_EQUAL(0b01010101, packetData[1]);
    CHECK_EQUAL(0b11000011, packetData[2]);
    CHECK_EQUAL(0b00011001, packetData[3]);
    CHECK_EQUAL(0b00000010, packetData[4]);
    CHECK_EQUAL(0b00000011, packetData[5]);

    uint16_t expectedCrc = IsoChecksum_calculate(packetData, packetSize - SPACE_PACKET_ERROR_CONTROL_SIZE);

    CHECK_EQUAL((expectedCrc >> 8u) & 0xFF, packetData[packetSize - 2]);
    CHECK_EQUAL(expectedCrc & 0xFF, packetData[packetSize - 1]);
}

TEST(CCSDSPacketizer, CCSDSDepacketizeTelemetry)
{
    Packetizer packetizer = { 8 };

    packetData[0] = 0b00001101;
    packetData[1] = 0b00000101;
    packetData[4] = 0b00000010;
    packetData[5] = 0b00000011;

    uint16_t crc = IsoChecksum_calculate(packetData, packetSize - SPACE_PACKET_ERROR_CONTROL_SIZE);
    packetData[packetSize - 2] = (crc >> 8u) & 0xFF;
    packetData[packetSize - 1] = crc & 0xFF;

    uint16_t receivedDestination = 0;
    size_t receivedDataOffset = 0;
    size_t receivedDataSize = 0;
    int32_t receivedErrorCode = 0;

    auto success = CCSDS_Packetizer_depacketize(&packetizer,
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
    CHECK_EQUAL(CCSDS_SPACE_PACKET_PRIMARY_HEADER_SIZE, receivedDataOffset);
    CHECK_EQUAL(dataSize, receivedDataSize);
}

TEST(CCSDSPacketizer, CCSDSDepacketizeTelecommand)
{
    Packetizer packetizer = { 7 };

    packetData[0] = 0b00011110;
    packetData[1] = 0b00000110;
    packetData[4] = 0b00000010;
    packetData[5] = 0b00000011;

    uint16_t crc = IsoChecksum_calculate(packetData, packetSize - SPACE_PACKET_ERROR_CONTROL_SIZE);
    packetData[packetSize - 2] = (crc >> 8u) & 0xFF;
    packetData[packetSize - 1] = crc & 0xFF;

    uint16_t receivedDestination = 0;
    size_t receivedDataOffset = 0;
    size_t receivedDataSize = 0;
    int32_t receivedErrorCode = 0;

    auto success = CCSDS_Packetizer_depacketize(&packetizer,
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
    CHECK_EQUAL(CCSDS_SPACE_PACKET_PRIMARY_HEADER_SIZE, receivedDataOffset);
    CHECK_EQUAL(dataSize, receivedDataSize);
}

TEST(CCSDSPacketizer, CCSDSPacketizeDepacketizeTelecommand)
{
    const uint16_t destination = 1365;
    const uint16_t sequenceCount = 793;
    Packetizer packetizer = { sequenceCount };

    auto packetizeResultSize = CCSDS_Packetizer_packetize(&packetizer,
                                                          Packetizer_PacketType_Telecommand,
                                                          0,
                                                          destination,
                                                          packetData,
                                                          CCSDS_SPACE_PACKET_PRIMARY_HEADER_SIZE,
                                                          dataSize);

    uint16_t receivedDestination = 0;
    size_t receivedDataOffset = 0;
    size_t receivedDataSize = 0;
    int32_t receivedErrorCode = 0;

    auto depacketizeSuccess = CCSDS_Packetizer_depacketize(&packetizer,
                                                           Packetizer_PacketType_Telecommand,
                                                           packetData,
                                                           packetSize,
                                                           NULL,
                                                           &receivedDestination,
                                                           &receivedDataOffset,
                                                           &receivedDataSize,
                                                           &receivedErrorCode);

    CHECK(depacketizeSuccess);
    CHECK_EQUAL(packetSize, packetizeResultSize);
    CHECK_EQUAL(CCSDS_SPACE_PACKET_PRIMARY_HEADER_SIZE, receivedDataOffset);
    CHECK_EQUAL(0, receivedErrorCode);
    CHECK_EQUAL(dataSize, receivedDataSize);
    CHECK_EQUAL(destination, receivedDestination);

    uint16_t expectedCrc = IsoChecksum_calculate(packetData, packetSize - SPACE_PACKET_ERROR_CONTROL_SIZE);

    CHECK_EQUAL((expectedCrc >> 8u) & 0xFF, packetData[packetSize - 2]);
    CHECK_EQUAL(expectedCrc & 0xFF, packetData[packetSize - 1]);
}

TEST_GROUP(CCSDSPacketizerInternal)
{
    uint8_t packetData[CCSDS_SPACE_PACKET_PRIMARY_HEADER_SIZE];

    void setup() { memset(packetData, 0, CCSDS_SPACE_PACKET_PRIMARY_HEADER_SIZE); }
};

TEST(CCSDSPacketizerInternal, CCSDSApidMin)
{
    writeCCSDSPacketId(packetData, Packetizer_PacketType_Telecommand, 0);

    CHECK_EQUAL(0b00011000, packetData[0]);
    CHECK_EQUAL(0b00000000, packetData[1]);
}

TEST(CCSDSPacketizerInternal, CCSDSApidStandard)
{
    writeCCSDSPacketId(packetData, Packetizer_PacketType_Telecommand, 685);

    CHECK_EQUAL(0b00011010, packetData[0]);
    CHECK_EQUAL(0b10101101, packetData[1]);
}

TEST(CCSDSPacketizerInternal, CCSDSApidMax)
{
    writeCCSDSPacketId(packetData, Packetizer_PacketType_Telecommand, SPACE_PACKET_MAX_APID);
    CHECK_EQUAL(0b00011111, packetData[0]);
    CHECK_EQUAL(0b11111111, packetData[1]);
}

TEST(CCSDSPacketizerInternal, CCSDSSequenceCounterMin)
{
    Packetizer packetizer = { 0 };
    writeCCSDSPacketSequenceControl(packetData, &packetizer);

    CHECK_EQUAL(0b11000000, packetData[2]);
    CHECK_EQUAL(0b00000000, packetData[3]);
}

TEST(CCSDSPacketizerInternal, CCSDSSequenceCounterStandard)
{
    Packetizer packetizer = { 718 };
    writeCCSDSPacketSequenceControl(packetData, &packetizer);

    CHECK_EQUAL(0b11000010, packetData[2]);
    CHECK_EQUAL(0b11001110, packetData[3]);
}

TEST(CCSDSPacketizerInternal, CCSDSSequenceCounterMax)
{
    Packetizer packetizer = { SPACE_PACKET_MAX_PACKET_SEQUENCE_COUNT };
    writeCCSDSPacketSequenceControl(packetData, &packetizer);

    CHECK_EQUAL(0b11111111, packetData[2]);
    CHECK_EQUAL(0b11111111, packetData[3]);
}

TEST(CCSDSPacketizerInternal, CCSDSDataSizeMin)
{
    writeCCSDSPacketDataLength(packetData, 1);

    CHECK_EQUAL(0b00000000, packetData[4]);
    CHECK_EQUAL(0b00000000, packetData[5]);
}

TEST(CCSDSPacketizerInternal, CCSDSDataSizeStandard)
{
    writeCCSDSPacketDataLength(packetData, 43606);

    CHECK_EQUAL(0b10101010, packetData[4]);
    CHECK_EQUAL(0b01010101, packetData[5]);
}

TEST(CCSDSPacketizerInternal, CCSDSDataSizeMax)
{
    writeCCSDSPacketDataLength(packetData, CCSDS_SPACE_PACKET_MAX_PACKET_DATA_SIZE);

    CHECK_EQUAL(0b11111111, packetData[4]);
    CHECK_EQUAL(0b11111111, packetData[5]);
}
