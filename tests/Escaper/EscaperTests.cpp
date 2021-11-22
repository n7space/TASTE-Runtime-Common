#include "CppUTest/TestHarness.h"

extern "C"
{
#include <Escaper.h>
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

    static constexpr int LENGTH_TEST_ARRAY_1{ 14 };
    static constexpr int LENGTH_TEST_ARRAY_2{ 16 };

    uint8_t TEST_ARRAY_1[LENGTH_TEST_ARRAY_1];
    uint8_t TEST_ARRAY_2[LENGTH_TEST_ARRAY_2];

    Escaper* escaper{ nullptr };

    void setup()
    {
        // always create "fresh" structure before test
        escaper = new Escaper;
    }

    void teardown() { delete escaper; }

    static void ReceivePacket(uint8_t* const buffer, size_t length) {}

    void VerifyEncoding(const Escaper* const escaper, size_t encodedLength)
    {
        BYTES_EQUAL_TEXT(START_BYTE, escaper->m_send_packet_buffer[0], "No START_BYTE");
        BYTES_EQUAL_TEXT(STOP_BYTE, escaper->m_send_packet_buffer[encodedLength - 1], "NO STOP_BYTE");

        for(uint8_t j = 1; j < encodedLength - 1; j++) {
            switch(escaper->m_send_packet_buffer[j]) {
                case START_BYTE:
                    FAIL("!!!! Non-escaped START_BYTE !!!! \n\r");
                    break;
                case STOP_BYTE:
                    FAIL("!!!! Non-escaped START_BYTE !!!! \n\r");
                    break;
                case ESCAPE_BYTE:
                    switch(escaper->m_send_packet_buffer[j + 1]) {
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
        for(int j = 0; (j < size && j < sizeof(escaper->m_recv_packet_buffer)); j++) {
            BYTES_EQUAL(buffer[j], escaper->m_recv_packet_buffer[j]);
        }
    }

    void GenerateRandTestCase(uint8_t * buffer, int size)
    {
        for(int i = 0; i < size; i++) {
            buffer[i] = rand() % ESCAPE_BYTE + 1; /// Generate random "non-special" byte
        }

        /// In random places, put bytes that need to be escaped
        static constexpr int NUMBER_OF_START_BYTES{ 2 };
        static constexpr int NUMBER_OF_STOP_BYTES{ 3 };
        static constexpr int NUMBER_OF_ESCAPE_BYTES{ 1 };

        for(int i = 0; i < NUMBER_OF_START_BYTES; i++) {
            buffer[rand() % size] = START_BYTE;
        }

        for(int i = 0; i < NUMBER_OF_STOP_BYTES; i++) {
            buffer[rand() % size] = STOP_BYTE;
        }

        for(int i = 0; i < NUMBER_OF_ESCAPE_BYTES; i++) {
            buffer[rand() % size] = ESCAPE_BYTE;
        }
    }
};

TEST(Escaper, encodingNoSpecialBytes1)
{
    size_t index{ 0 }, encodedLength;

    Escaper_start_encoder(escaper);
    encodedLength = Escaper_encode_packet(escaper, NO_SPECIAL_CHARS1, sizeof(NO_SPECIAL_CHARS1), &index);
    VerifyEncoding(escaper, encodedLength);
};

TEST(Escaper, encodingNoSpecialBytes2)
{
    size_t index{ 0 }, encodedLength;

    Escaper_start_encoder(escaper);
    encodedLength = Escaper_encode_packet(escaper, NO_SPECIAL_CHARS2, sizeof(NO_SPECIAL_CHARS2), &index);
    VerifyEncoding(escaper, encodedLength);
};

TEST(Escaper, encodingOnlySTART_BYTES)
{
    size_t index{ 0 }, encodedLength;

    Escaper_start_encoder(escaper);
    encodedLength = Escaper_encode_packet(escaper, ONLY_START_BYTES, sizeof(ONLY_START_BYTES), &index);
    VerifyEncoding(escaper, encodedLength);
};

TEST(Escaper, encodingOnlySTOP_BYTES)
{
    size_t index{ 0 }, encodedLength;

    Escaper_start_encoder(escaper);
    encodedLength = Escaper_encode_packet(escaper, ONLY_STOP_BYTES, sizeof(ONLY_STOP_BYTES), &index);
    VerifyEncoding(escaper, encodedLength);
};

TEST(Escaper, encodingOnlyEscape_BYTES)
{
    size_t index{ 0 }, encodedLength;

    Escaper_start_encoder(escaper);
    encodedLength = Escaper_encode_packet(escaper, ONLY_ESCAPE_BYTES, sizeof(ONLY_ESCAPE_BYTES), &index);
    VerifyEncoding(escaper, encodedLength);
};

TEST(Escaper, encodingRandChar1)
{
    size_t index{ 0 }, encodedLength;
    GenerateRandTestCase(TEST_ARRAY_1, LENGTH_TEST_ARRAY_1);
    Escaper_start_encoder(escaper);
    encodedLength = Escaper_encode_packet(escaper, TEST_ARRAY_1, sizeof(TEST_ARRAY_1), &index);
    VerifyEncoding(escaper, encodedLength);
};

TEST(Escaper, encodingRandChar2)
{
    size_t index{ 0 }, encodedLength;
    GenerateRandTestCase(TEST_ARRAY_2, LENGTH_TEST_ARRAY_2);
    Escaper_start_encoder(escaper);
    encodedLength = Escaper_encode_packet(escaper, TEST_ARRAY_2, sizeof(TEST_ARRAY_2), &index);
    VerifyEncoding(escaper, encodedLength);
};

TEST(Escaper, parsingNoSpecialBytes1)
{
    size_t encodedLength;
    encodedLength = SetupParsing(escaper, NO_SPECIAL_CHARS1, sizeof(NO_SPECIAL_CHARS1));
    Escaper_start_decoder(escaper);
    Escaper_decode_packet(escaper, escaper->m_send_packet_buffer, encodedLength, ReceivePacket);
    VerifyParsing(escaper, NO_SPECIAL_CHARS1, sizeof(NO_SPECIAL_CHARS1));
};

TEST(Escaper, parsingNoSpecialBytes2)
{
    size_t encodedLength;
    encodedLength = SetupParsing(escaper, NO_SPECIAL_CHARS2, sizeof(NO_SPECIAL_CHARS2));
    Escaper_start_decoder(escaper);
    Escaper_decode_packet(escaper, escaper->m_send_packet_buffer, encodedLength, ReceivePacket);
    VerifyParsing(escaper, NO_SPECIAL_CHARS2, sizeof(NO_SPECIAL_CHARS2));
};

TEST(Escaper, parsingOnlySTART_BYTES)
{
    size_t encodedLength;
    encodedLength = SetupParsing(escaper, ONLY_START_BYTES, sizeof(ONLY_START_BYTES));
    Escaper_start_decoder(escaper);
    Escaper_decode_packet(escaper, escaper->m_send_packet_buffer, encodedLength, ReceivePacket);
    VerifyParsing(escaper, ONLY_START_BYTES, sizeof(ONLY_START_BYTES));
};

TEST(Escaper, parsingOnlySTOP_BYTES)
{
    size_t encodedLength;
    encodedLength = SetupParsing(escaper, ONLY_STOP_BYTES, sizeof(ONLY_STOP_BYTES));
    Escaper_start_decoder(escaper);
    Escaper_decode_packet(escaper, escaper->m_send_packet_buffer, encodedLength, ReceivePacket);
    VerifyParsing(escaper, ONLY_STOP_BYTES, sizeof(ONLY_STOP_BYTES));
};

TEST(Escaper, parsingOnlyEscape_BYTES)
{
    size_t encodedLength;
    encodedLength = SetupParsing(escaper, ONLY_ESCAPE_BYTES, sizeof(ONLY_ESCAPE_BYTES));
    Escaper_start_decoder(escaper);
    Escaper_decode_packet(escaper, escaper->m_send_packet_buffer, encodedLength, ReceivePacket);
    VerifyParsing(escaper, ONLY_ESCAPE_BYTES, sizeof(ONLY_ESCAPE_BYTES));
};

TEST(Escaper, parsingRandChar1)
{
    size_t encodedLength;
    GenerateRandTestCase(TEST_ARRAY_1, LENGTH_TEST_ARRAY_1);
    encodedLength = SetupParsing(escaper, TEST_ARRAY_1, sizeof(TEST_ARRAY_1));
    Escaper_start_decoder(escaper);
    Escaper_decode_packet(escaper, escaper->m_send_packet_buffer, encodedLength, ReceivePacket);
    VerifyParsing(escaper, TEST_ARRAY_1, LENGTH_TEST_ARRAY_1);
};

TEST(Escaper, parsingRandChar2)
{
    size_t encodedLength;
    GenerateRandTestCase(TEST_ARRAY_2, LENGTH_TEST_ARRAY_2);
    encodedLength = SetupParsing(escaper, TEST_ARRAY_2, sizeof(TEST_ARRAY_2));
    Escaper_start_decoder(escaper);
    Escaper_decode_packet(escaper, escaper->m_send_packet_buffer, encodedLength, ReceivePacket);
    VerifyParsing(escaper, TEST_ARRAY_2, LENGTH_TEST_ARRAY_2);
};
