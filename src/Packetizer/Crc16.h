#ifndef PACKETIZER_CRC_H
#define PACKETIZER_CRC_H

#include <stddef.h>
#include <stdint.h>

/**
 * @file  Crc.h
 * @brief Component responsible for CRC checksum calculation as defined in ECSS-E-70-41C Annex B.1.
 */

/**
 * @brief Initial value of CRC checksum (checksum of empty data collection).
 */
enum { Crc16_InitialValue = 0xFFFF };

/**
 * @brief Calculates CRC checksum for given data block.
 *        Checksum is calculated according to as ECSS-E-70-41C Annex B.1.
 *
 * @param [in] data     Pointer to start of data block. Can't be NULL.
 * @param [in] length   Data block length in bytes.
 *
 * @return  Calculated checksum.
 */
uint16_t calculateCrc16(const uint8_t* const data, const size_t length);

#endif // PACKETIZER_CRC_H
