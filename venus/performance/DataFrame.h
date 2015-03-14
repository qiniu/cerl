#ifndef CERL_TEST_DATA_FRAME_H
#define CERL_TEST_DATA_FRAME_H

#include <venus/Debug.h>
#include <venus/Mail.h>

#pragma pack(1)

enum { TEST_DATA_SIZE = 100 };

struct DataBlock
{
	UINT8 data[TEST_DATA_SIZE];
};

struct TestData
{
	cerl::MailHeader header;
	UINT16 type;
	DataBlock data;
	UINT16 end;
};

#pragma pack()

#endif
