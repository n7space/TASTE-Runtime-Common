// TODO: ADD LICENSE

#ifndef PACKETIZER_INTERNAL_H
#define PACKETIZER_INTERNAL_H

void writePacketId(uint8_t* const dataPointer, const Packetizer_PacketType packetType, const uint16_t source);
void writePacketSequenceControl(uint8_t* const dataPointer, const Packetizer* const packetizer);
void writePacketDataLength(uint8_t* const dataPointer, const uint32_t dataSize);

#endif // PACKETIZER_INTERNAL_H
