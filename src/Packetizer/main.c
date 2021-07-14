#include <stdio.h>
#include <stdlib.h>

#include "Packetizer.h"
#include "SpacePacket.h"

int
main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    Packetizer packetizer = { .packetSequenceCount = 5 };

    uint8_t data[SPACE_PACKET_PRIMARY_HEADER_SIZE_BYTES + 6542 + SPACE_PACKET_ERROR_CONTROL_SIZE_BYTES] = { 0 };

    size_t packetSize = Packetizer_packetize(
            &packetizer, Packetizer_PacketType_Telecommand, 0, 0, data, SPACE_PACKET_PRIMARY_HEADER_SIZE_BYTES, 6542);
    (void)packetSize;

    printf("%d\n", packetizer.packetSequenceCount);
    printf("%d\n", data[0] | (data[1] << 8));
    printf("%d\n", data[2] | (data[3] << 8));
    printf("%d\n", data[4] | (data[5] << 8));

    return EXIT_SUCCESS;
}
