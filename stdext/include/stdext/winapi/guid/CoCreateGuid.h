/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: stdext/winapi/guid/CoCreateGuid.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:08:57
// 
// $Id: CoCreateGuid.h,v 1.6 2007/01/10 09:38:10 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_WINAPI_GUID_COCREATEGUID_H
#define STDEXT_WINAPI_GUID_COCREATEGUID_H

#ifndef STDEXT_WINAPI_WTYPES_H
#include "../wtypes.h"
#endif

// -------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>

namespace winapi_detail {

/* Number of 100ns ticks per clock tick. To be safe, assume that the clock
* resolution is at least 1000 * 100 * (1/1000000) = 1/10 of a second */
#define TICKS_PER_CLOCK_TICK        1000
#define SECSPERDAY                  86400
#define TICKSPERSEC                 10000000
#define MAXINTERFACES               16
#define ADDRESS_BYTES_NEEDED        6
#define MAX_ADAPTER_ADDRESS_LENGTH  8

/* GUID system time starts at October 15, 1582 */
#define SECS_15_OCT_1582_TO_1601    ((17 + 30 + 31 + 365 * 18 + 5) * SECSPERDAY)
#define TICKS_15_OCT_1582_TO_1601   ((ULONGLONG)SECS_15_OCT_1582_TO_1601 * TICKSPERSEC)
#define SECS_1601_TO_1970           ((369 * 365 + 89) * (ULONGLONG)SECSPERDAY)
#define TICKS_1601_TO_1970          (SECS_1601_TO_1970 * TICKSPERSEC)

inline int
NtQuerySystemTime(PLARGE_INTEGER Time)
{
	struct timeval now;
	gettimeofday(&now, 0);
	Time->QuadPart = now.tv_sec * (ULONGLONG)TICKSPERSEC + TICKS_1601_TO_1970;
	Time->QuadPart += now.tv_usec * 10;

	return 0;
}

inline void
GetSystemTimeAsFileTime(FILETIME *time) /* [out] Destination for the current utc time */
{
	LARGE_INTEGER t;
	NtQuerySystemTime(&t);
	time->dwLowDateTime = t.u.LowPart;
	time->dwHighDateTime = t.u.HighPart;
}

inline void
RPC_UuidGetSystemTime(ULONGLONG *time)
{
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	*time = ((ULONGLONG)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
	*time += TICKS_15_OCT_1582_TO_1601;
}

inline int
GetMacAddress(BYTE *address)
{
	register int fd, intrface;
	struct ifreq buf[MAXINTERFACES];
	struct ifconf ifc;
	int i;
	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = (caddr_t)buf;

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cpm: socket");
		close(fd);
		return -1;
	}

	if (ioctl(fd, SIOCGIFCONF, (char *)&ifc)) {
		perror("cpm: ioctl");
		close(fd);
		return -2;
	}

	intrface = ifc.ifc_len / sizeof(struct ifreq);
	if (intrface <= 1) {
		printf("interface num is intrface=%d\n", intrface);
		close(fd);
		return -3;
	}
	while (intrface-- > 0) {
		if (strcmp("eth0", buf[intrface].ifr_name))
			continue;
		if (!(ioctl(fd, SIOCGIFHWADDR, (char *) &buf[intrface]))) {
			for (i=0; i<ADDRESS_BYTES_NEEDED; i++)
				address[i] = (BYTE)buf[intrface].ifr_hwaddr.sa_data[i];
		} else {
			char str[256];
			sprintf(str, "cpm: ioctl device %s", buf[intrface].ifr_name);
			perror(str);
		}
	}
	close(fd);
	return 0;
}

inline int
RPC_UuidGetNodeAddress(BYTE *address)
{
	int i;
	if (!GetMacAddress(address)) {
		return 0;
	} else {
		/* We can't get a hardware address, just use random numbers.
		* Set the multicast bit to prevent conflicts with real cards. */
		for (i = 0; i < ADDRESS_BYTES_NEEDED; i++) {
			address[i] = rand() & 0xff;
		}
		address[0] |= 0x01;
		return 1;
	}
}

} // namespace winapi_detail

inline HRESULT
CoCreateGuid(GUID *Uuid)
{
	using namespace winapi_detail;

	static int initialised, count;
	ULONGLONG time;
	static ULONGLONG timelast;
	static WORD sequence;
	static DWORD status;
	static BYTE address[MAX_ADAPTER_ADDRESS_LENGTH];
	if (!initialised) {
		RPC_UuidGetSystemTime(&timelast);
		count = TICKS_PER_CLOCK_TICK;
		sequence = ((rand() & 0xff) << 8) + (rand() & 0xff);
		sequence &= 0x1fff;
		status = RPC_UuidGetNodeAddress(address);
		initialised = 1;
	}
	/* Generate time element of the GUID. Account for going faster
	* than our clock as well as the clock going backwards. */
	while (1) {
		RPC_UuidGetSystemTime(&time);
		if (time > timelast) {
			count = 0;
			break;
		}
		if (time < timelast) {
			sequence = (sequence + 1) & 0x1fff;
			count = 0;
			break;
		}
		if (count < TICKS_PER_CLOCK_TICK) {
			count++;
			break;
		}
	}
	timelast = time;
	time += count;
	/* Pack the information into the GUID structure. */
	Uuid->Data1  = (unsigned long)(time & 0xffffffff);
	Uuid->Data2  = (unsigned short)((time >> 32) & 0xffff);
	Uuid->Data3  = (unsigned short)((time >> 48) & 0x0fff);
	/* This is a version 1 GUID */
	Uuid->Data3 |= (1 << 12);
	Uuid->Data4[0]  = sequence & 0xff;
	Uuid->Data4[1]  = (sequence & 0x3f00) >> 8;
	Uuid->Data4[1] |= 0x80;
	Uuid->Data4[2] = address[0];
	Uuid->Data4[3] = address[1];
	Uuid->Data4[4] = address[2];
	Uuid->Data4[5] = address[3];
	Uuid->Data4[6] = address[4];
	Uuid->Data4[7] = address[5];
	return status;
}

// -------------------------------------------------------------------------

#endif /* STDEXT_WINAPI_GUID_COCREATEGUID_H */
