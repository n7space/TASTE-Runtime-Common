/* Header file for function TimeService
 * Generated by TASTE (kazoo/templates/skeletons/c-header/function.tmplt)
 * DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
 */

#pragma once

#include "dataview-uniq.h"

#ifdef __cplusplus
extern "C" {
#endif


void timeservice_startup(void);

/* Provided interfaces */
void timeservice_PI_CfsTimestampCmp( const asn1SccCfsTimestamp *, const asn1SccCfsTimestamp *, asn1SccComparisonResult * );


void timeservice_PI_ClockStatus( asn1SccClockStatusEnum * );


void timeservice_PI_CucTimestamp( const asn1SccULongInteger *, asn1SccCucTimestamp * );


void timeservice_PI_CucTimestampCmp( const asn1SccCucTimestamp *, const asn1SccCucTimestamp *, asn1SccComparisonResult * );


void timeservice_PI_ObetTime( asn1SccULongInteger * );

/* Required interfaces */



#ifdef __cplusplus
}
#endif