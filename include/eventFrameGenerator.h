/* ========================================

   * File Name : eventFrameGenerator.h

   * Creation Date : 24-08-2015

   * Last Modified : Tue 25 Aug 2015 05:22:11 PM CEST

   * Created By : Rainer Schwemmer <rschwemm@cern.ch>
   
   * Modified by: Karel Ha <mathemage@gmail.com>

   ==========================================*/

#ifndef _EVENTFRAMEGENERATOR_H_
#define _EVENTFRAMEGENERATOR_H_

#include <cstdlib>
#include <cstdio>

struct MEPFragmentHeader
{
	u_int16_t sourceID;
	u_int8_t type;
	u_int8_t options;
	u_int32_t len;
	u_int64_t eventID;
	u_int16_t nEvents;
	u_int16_t padding1;
	u_int32_t padding2[11];
	u_int16_t offsets[];
};

struct FrameHeader
{
	u_int32_t numMEPs;
	u_int64_t dataLength;
};

class EventFrameGenerator
{
public:
	EventFrameGenerator(size_t rndBufferSize, u_int32_t seed);
	~EventFrameGenerator();

	void fillMemoryBlock(char* target, int64_t length, u_int16_t averageEventSize, size_t delta, size_t mepFactor, u_int16_t sourceID);
	void fillMetaBlock(char* target, int64_t length, u_int16_t averageEventSize, size_t delta, size_t mepFactor, u_int16_t sourceID);
	void fillSeparatedBlock(char* target, size_t dataLength, u_int16_t averageEventSize, size_t delta, u_int16_t sourceID, char* metaBlock, size_t metaLength);

private:
	size_t m_numInts;
	u_int16_t m_eventID[4];

	u_int16_t* m_randomFragmentSizes;
	size_t     m_randomFramgentsOffset;

};

#endif
