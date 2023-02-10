#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport_c.h"

extern "C"
{
#include <Broker.h>
#include <system_spec.h>
}

void
testErrorDetectedCallback(const Broker_ErrorType errorType, uint8_t* const data, const size_t length)
{
    mock_c()->actualCall("testErrorDetectedCallback")
            ->withIntParameters("errorType", errorType)
            ->withPointerParameters("data", data)
            ->withIntParameters("length", length);
}

void
deliverFunction(const uint8_t* const data, const size_t length)
{
    mock_c()->actualCall("deliverFunction");
}

extern "C"
{
    enum PacketizerCfg bus_to_packetizer_cfg[] = { PACKETIZER_DEFAULT, PACKETIZER_DEFAULT };
    deliver_function interface_to_deliver_function[INTERFACE_MAX_ID];
    void Broker_acquire_lock() { mock_c()->actualCall("Broker_acquire_lock"); }
    void Broker_release_lock() { mock_c()->actualCall("Broker_release_lock"); }
}

TEST_GROUP(Broker){ void setup(){ interface_to_deliver_function[INTERFACE_INVALID_ID] = &deliverFunction;
}
void
teardown()
{
}
}
;

TEST(Broker, tooSmall)
{
    enum SystemBus all_busess[] = { BUS_1, BUS_1 };
    Broker_initialize(all_busess);

    Broker_register_error_callback(&testErrorDetectedCallback);

    uint8_t data[] = { 0 };
    const size_t length = sizeof(data);

    mock_c()->expectOneCall("Broker_acquire_lock");
    mock_c()->expectOneCall("testErrorDetectedCallback")
            ->withIntParameters("errorType", Broker_ErrorType_PacketTooSmall)
            ->withPointerParameters("data", data)
            ->withIntParameters("length", 1);
    mock_c()->expectOneCall("Broker_release_lock");

    Broker_receive_packet(BUS_1, data, length);
};

TEST(Broker, InvalidCrc)
{
    enum SystemBus all_busess[] = { BUS_1, BUS_1 };
    Broker_initialize(all_busess);

    Broker_register_error_callback(&testErrorDetectedCallback);

    uint8_t data[] = { 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0 };
    const size_t length = sizeof(data);

    mock_c()->expectOneCall("Broker_acquire_lock");
    mock_c()->expectOneCall("testErrorDetectedCallback")
            ->withIntParameters("errorType", Broker_ErrorType_IncorrectCrc16)
            ->withPointerParameters("data", data)
            ->withIntParameters("length", 12);
    mock_c()->expectOneCall("Broker_release_lock");

    Broker_receive_packet(BUS_1, data, length);
};

TEST(Broker, NoError)
{
    enum SystemBus all_busess[] = { BUS_1, BUS_1 };
    Broker_initialize(all_busess);

    Broker_register_error_callback(&testErrorDetectedCallback);

    uint8_t data[] = { 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0xA8, 0xF1 };
    const size_t length = sizeof(data);

    mock_c()->expectOneCall("Broker_acquire_lock");
    mock_c()->expectOneCall("deliverFunction");
    mock_c()->expectNoCall("testErrorDetectedCallback");
    mock_c()->expectOneCall("Broker_release_lock");

    Broker_receive_packet(BUS_1, data, length);
};
