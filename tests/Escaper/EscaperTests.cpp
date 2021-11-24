#include "CppUTest/TestHarness.h"

extern "C"
{
#include <Escaper.h>
#include <EscaperInternal.h>
}

void* bus_to_driver_private_data[16];
void* bus_to_driver_send_function[16];
void* interface_to_deliver_function[16];

TEST_GROUP(Escaper)
{
    static constexpr uint8_t NO_SPECIAL_CHARS1[]{ "HELLO" };
    static constexpr uint8_t NO_SPECIAL_CHARS2[] = { "Goodbye!" };
    static constexpr uint8_t ONLY_START_BYTES[] = { START_BYTE, START_BYTE, START_BYTE, START_BYTE,
                                                    START_BYTE, START_BYTE, START_BYTE };
    static constexpr uint8_t ONLY_STOP_BYTES[] = { STOP_BYTE, STOP_BYTE, STOP_BYTE, STOP_BYTE, STOP_BYTE };
    static constexpr uint8_t ONLY_ESCAPE_BYTES[] = { ESCAPE_BYTE, ESCAPE_BYTE, ESCAPE_BYTE,
                                                     ESCAPE_BYTE, ESCAPE_BYTE, ESCAPE_BYTE };

    static constexpr uint8_t TEST_ARRAY_1[] = { 5,         123,       START_BYTE, 21,          37, 234,
                                                STOP_BYTE, STOP_BYTE, 2,          ESCAPE_BYTE, 46, 13 };
    static constexpr uint8_t TEST_ARRAY_2[] = { STOP_BYTE, 21,  87,         START_BYTE, ESCAPE_BYTE, 43, 98,
                                                122,       197, START_BYTE, 252,        2,           8 };

    Escaper escaper;

    static constexpr size_t ENCODED_PACKET_MAX_SIZE{ 20 };
    static constexpr size_t DECODED_PACKET_MAX_SIZE{ 20 };
    uint8_t encodedPacketBuffer[ENCODED_PACKET_MAX_SIZE];
    uint8_t decodedPacketBuffer[DECODED_PACKET_MAX_SIZE];

    void setup() override
    {
        Escaper_init(
                &escaper, encodedPacketBuffer, ENCODED_PACKET_MAX_SIZE, decodedPacketBuffer, DECODED_PACKET_MAX_SIZE);
    };

    static void ReceivePacketFn(uint8_t* const buffer, size_t length) {}

    void VerifyEncoding(const Escaper* const escaper, size_t encodedLength)
    {
        BYTES_EQUAL_TEXT(START_BYTE, escaper->m_encoded_packet_buffer[0], "No START_BYTE");
        BYTES_EQUAL_TEXT(STOP_BYTE, escaper->m_encoded_packet_buffer[encodedLength - 1], "No STOP_BYTE");

        for(uint8_t j = 1; j < encodedLength - 1; j++) {
            switch(escaper->m_encoded_packet_buffer[j]) {
                case START_BYTE:
                    FAIL("!!!! Non-escaped START_BYTE !!!! \n\r");
                    break;
                case STOP_BYTE:
                    FAIL("!!!! Non-escaped START_BYTE !!!! \n\r");
                    break;
                case ESCAPE_BYTE:
                    switch(escaper->m_encoded_packet_buffer[j + 1]) {
                        case START_BYTE:
                        case STOP_BYTE:
                        case ESCAPE_BYTE:
                            j++;
                            break;
                        default:
                            FAIL("!!!! Non-escaped ESCAPE_BYTE !!!!");
                            break;
                    }
            }
        }
    }

    size_t SetupParsing(Escaper* const escaper, const uint8_t* const buffer, int size)
    {
        size_t index{ 0 }, encodedLength;
        Escaper_start_encoder(escaper);
        encodedLength = Escaper_encode_packet(escaper, buffer, size, &index);
        VerifyEncoding(escaper, encodedLength);

        return encodedLength;
    }

    void VerifyParsing(const Escaper* const escaper, const uint8_t* const buffer, int size)
    {
        for(int j = 0; (j < size && j < escaper->m_decoded_packet_max_size); j++) {
            BYTES_EQUAL(buffer[j], escaper->m_decoded_packet_buffer[j]);
        }
    }
};

TEST(Escaper, encodingNoSpecialBytes1)
{
    size_t index{ 0 }, encodedLength;

    Escaper_start_encoder(&escaper);
    encodedLength = Escaper_encode_packet(&escaper, NO_SPECIAL_CHARS1, sizeof(NO_SPECIAL_CHARS1), &index);
    VerifyEncoding(&escaper, encodedLength);
};

TEST(Escaper, encodingNoSpecialBytes2)
{
    size_t index{ 0 }, encodedLength;

    Escaper_start_encoder(&escaper);
    encodedLength = Escaper_encode_packet(&escaper, NO_SPECIAL_CHARS2, sizeof(NO_SPECIAL_CHARS2), &index);
    VerifyEncoding(&escaper, encodedLength);
};

TEST(Escaper, encodingOnlySTART_BYTES)
{
    size_t index{ 0 }, encodedLength;

    Escaper_start_encoder(&escaper);
    encodedLength = Escaper_encode_packet(&escaper, ONLY_START_BYTES, sizeof(ONLY_START_BYTES), &index);
    VerifyEncoding(&escaper, encodedLength);
};

TEST(Escaper, encodingOnlySTOP_BYTES)
{
    size_t index{ 0 }, encodedLength;

    Escaper_start_encoder(&escaper);
    encodedLength = Escaper_encode_packet(&escaper, ONLY_STOP_BYTES, sizeof(ONLY_STOP_BYTES), &index);
    VerifyEncoding(&escaper, encodedLength);
};

TEST(Escaper, encodingOnlyEscape_BYTES)
{
    size_t index{ 0 }, encodedLength;

    Escaper_start_encoder(&escaper);
    encodedLength = Escaper_encode_packet(&escaper, ONLY_ESCAPE_BYTES, sizeof(ONLY_ESCAPE_BYTES), &index);
    VerifyEncoding(&escaper, encodedLength);
};

TEST(Escaper, encodingByteArray1)
{
    size_t index{ 0 }, encodedLength;
    Escaper_start_encoder(&escaper);
    encodedLength = Escaper_encode_packet(&escaper, TEST_ARRAY_1, sizeof(TEST_ARRAY_1), &index);
    VerifyEncoding(&escaper, encodedLength);
};

TEST(Escaper, encodingByteArray2)
{
    size_t index{ 0 }, encodedLength;
    Escaper_start_encoder(&escaper);
    encodedLength = Escaper_encode_packet(&escaper, TEST_ARRAY_2, sizeof(TEST_ARRAY_2), &index);
    VerifyEncoding(&escaper, encodedLength);
};

TEST(Escaper, parsingNoSpecialBytes1)
{
    size_t encodedLength;
    encodedLength = SetupParsing(&escaper, NO_SPECIAL_CHARS1, sizeof(NO_SPECIAL_CHARS1));
    Escaper_start_decoder(&escaper);
    Escaper_decode_packet(&escaper, escaper.m_encoded_packet_buffer, encodedLength, ReceivePacketFn);
    VerifyParsing(&escaper, NO_SPECIAL_CHARS1, sizeof(NO_SPECIAL_CHARS1));
};

TEST(Escaper, parsingNoSpecialBytes2)
{
    size_t encodedLength;
    encodedLength = SetupParsing(&escaper, NO_SPECIAL_CHARS2, sizeof(NO_SPECIAL_CHARS2));
    Escaper_start_decoder(&escaper);
    Escaper_decode_packet(&escaper, escaper.m_encoded_packet_buffer, encodedLength, ReceivePacketFn);
    VerifyParsing(&escaper, NO_SPECIAL_CHARS2, sizeof(NO_SPECIAL_CHARS2));
};

TEST(Escaper, parsingOnlySTART_BYTES)
{
    size_t encodedLength;
    encodedLength = SetupParsing(&escaper, ONLY_START_BYTES, sizeof(ONLY_START_BYTES));
    Escaper_start_decoder(&escaper);
    Escaper_decode_packet(&escaper, escaper.m_encoded_packet_buffer, encodedLength, ReceivePacketFn);
    VerifyParsing(&escaper, ONLY_START_BYTES, sizeof(ONLY_START_BYTES));
};

TEST(Escaper, parsingOnlySTOP_BYTES)
{
    size_t encodedLength;
    encodedLength = SetupParsing(&escaper, ONLY_STOP_BYTES, sizeof(ONLY_STOP_BYTES));
    Escaper_start_decoder(&escaper);
    Escaper_decode_packet(&escaper, escaper.m_encoded_packet_buffer, encodedLength, ReceivePacketFn);
    VerifyParsing(&escaper, ONLY_STOP_BYTES, sizeof(ONLY_STOP_BYTES));
};

TEST(Escaper, parsingOnlyEscape_BYTES)
{
    size_t encodedLength;
    encodedLength = SetupParsing(&escaper, ONLY_ESCAPE_BYTES, sizeof(ONLY_ESCAPE_BYTES));
    Escaper_start_decoder(&escaper);
    Escaper_decode_packet(&escaper, escaper.m_encoded_packet_buffer, encodedLength, ReceivePacketFn);
    VerifyParsing(&escaper, ONLY_ESCAPE_BYTES, sizeof(ONLY_ESCAPE_BYTES));
};

TEST(Escaper, parsingByteArray1)
{
    size_t encodedLength;
    encodedLength = SetupParsing(&escaper, TEST_ARRAY_1, sizeof(TEST_ARRAY_1));
    Escaper_start_decoder(&escaper);
    Escaper_decode_packet(&escaper, escaper.m_encoded_packet_buffer, encodedLength, ReceivePacketFn);
    VerifyParsing(&escaper, TEST_ARRAY_1, sizeof(TEST_ARRAY_1));
};

TEST(Escaper, parsingByteArray2)
{
    size_t encodedLength;
    encodedLength = SetupParsing(&escaper, TEST_ARRAY_2, sizeof(TEST_ARRAY_2));
    Escaper_start_decoder(&escaper);
    Escaper_decode_packet(&escaper, escaper.m_encoded_packet_buffer, encodedLength, ReceivePacketFn);
    VerifyParsing(&escaper, TEST_ARRAY_2, sizeof(TEST_ARRAY_2));
};
