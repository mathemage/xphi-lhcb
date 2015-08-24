/* ========================================

   * File Name : eventFrameGenerator.cpp

   * Creation Date : 24-08-2015

   * Last Modified : Mon 24 Aug 2015 02:03:11 PM CEST

   * Created By : Rainer Schwemmer <rschwemm@cern.ch>
   
   * Modified by: Karel Ha <mathemage@gmail.com>

   ==========================================*/

#include "../../include/eventFrameGenerator.h"
#include <malloc.h>

EventFrameGenerator::EventFrameGenerator(size_t rndBufferSize, u_int32_t seed)
{
  /* TODO replace with other random number generator
	sfmt_t sfmt;

	size_t numInts = rndBufferSize / 8;
	if(numInts < (SFMT_MEXP / 128 + 1) * 2)
		numInts = (SFMT_MEXP / 128 + 1) * 2;

	numInts += numInts % 2;
	m_numInts = numInts;


	m_eventID[0] = 0;
	m_eventID[1] = 0;
	m_eventID[2] = 0;
	m_eventID[3] = 0;

	sfmt_init_gen_rand(&sfmt, seed);

	m_randomFragmentSizes = (u_int16_t*)memalign (16, m_numInts * 8);
	sfmt_fill_array32(&sfmt, (u_int32_t*)m_randomFragmentSizes, m_numInts);
	m_randomFramgentsOffset = 0;
  */
}

void EventFrameGenerator::fillMemoryBlock(char* target, int64_t length, u_int16_t averageEventSize, size_t delta, size_t mepFactor, u_int16_t sourceID)
{
	u_int16_t* sizePointer = m_randomFragmentSizes;
	//Generate event fragments until we run out of space
	MEPFragmentHeader* header = (MEPFragmentHeader*)(target + sizeof(FrameHeader));
	int16_t newSize;
	int64_t blockSize = sizeof(FrameHeader);
	size_t numIters = 0;
	size_t totalMEPs = 0;
	while(true)
	{
		numIters++;
		blockSize += (sizeof(MEPFragmentHeader)- sizeof(u_int16_t*) + mepFactor * sizeof(u_int16_t));
		if((length - blockSize) < (long)((sizeof(MEPFragmentHeader)- sizeof(u_int16_t*) + mepFactor * sizeof(u_int16_t))))
		{
			blockSize -= (sizeof(MEPFragmentHeader)- sizeof(u_int16_t*) + mepFactor * sizeof(u_int16_t));
			break;
		}

		u_int32_t totalSize = 0;
		for(unsigned int i = 0; i < mepFactor; ++i)
		{
			//header->offsets[i] = totalSize;
			//newSize = averageEventSize;
			newSize = ((*(sizePointer++)) % (delta * 2)) - delta;
			newSize += averageEventSize;
			totalSize += newSize;
			//Wrap around
			if(++m_randomFramgentsOffset > m_numInts * 2)
			{
				m_randomFramgentsOffset = 0;
				sizePointer = m_randomFragmentSizes;
			}
			blockSize += newSize;
			if(length - blockSize < 0)
			{
				blockSize -= newSize;
				mepFactor = i;
				break;
			}
			header->offsets[i] = newSize;

		}
		header->nEvents = mepFactor;
		header->sourceID = sourceID;
		header->type = 1;
		header->len = totalSize + sizeof(struct MEPFragmentHeader) - sizeof(u_int16_t*) + (sizeof(u_int16_t) * mepFactor);
		header->eventID = 0;
		char* p = (char*)header;
		p += totalSize + sizeof(struct MEPFragmentHeader) - sizeof(u_int16_t*) + (sizeof(u_int16_t) * mepFactor);
		header = (MEPFragmentHeader*)p;
		++totalMEPs;
	}
	FrameHeader* frameHeader = (FrameHeader*)target;
	frameHeader->numMEPs = totalMEPs;
	frameHeader->dataLength = blockSize;

}

void EventFrameGenerator::fillMetaBlock(char* target, int64_t length, u_int16_t averageEventSize, size_t delta, size_t mepFactor, u_int16_t sourceID)
{
	u_int16_t* sizePointer = m_randomFragmentSizes;
	//Generate event fragments until we run out of space
	MEPFragmentHeader* header = (MEPFragmentHeader*)(target + sizeof(FrameHeader));
	int16_t newSize;
	int64_t blockSize = sizeof(FrameHeader);
	size_t numIters = 0;
	size_t totalMEPs = 0;
	while(true)
	{
		numIters++;
		blockSize += (sizeof(MEPFragmentHeader) + (sizeof(u_int16_t) * (mepFactor-1)));
		if((length - blockSize) < (long)((sizeof(MEPFragmentHeader) + (sizeof(u_int16_t) * (mepFactor-1)))))
		{
			blockSize -= (sizeof(MEPFragmentHeader) + (sizeof(u_int16_t) * (mepFactor-1)));
			break;
		}

		u_int32_t totalSize = 0;
		for(unsigned int i = 0; i < mepFactor; ++i)
		{
			//header->offsets[i] = totalSize;
			//newSize = averageEventSize;
			newSize = ((*(sizePointer++)) % (delta * 2)) - delta;
			newSize += averageEventSize;
			totalSize += newSize;
			//Wrap around
			if(++m_randomFramgentsOffset > m_numInts * 2)
			{
				m_randomFramgentsOffset = 0;
				sizePointer = m_randomFragmentSizes;
			}
			blockSize += newSize;
			if(length - blockSize < 0)
			{
				blockSize -= newSize;
				mepFactor = i;
				break;
			}
			header->offsets[i] = newSize;

		}
		header->nEvents = mepFactor;
		header->sourceID = sourceID;
		header->type = 1;
		header->len = totalSize + sizeof(struct MEPFragmentHeader) + (sizeof(u_int16_t) * (mepFactor-1));
		header->eventID = 0;
		++totalMEPs;
	}
	FrameHeader* frameHeader = (FrameHeader*)target;
	frameHeader->numMEPs = totalMEPs;
	frameHeader->dataLength = blockSize;
}

void EventFrameGenerator::fillSeparatedBlock(char* target, size_t dataLength, u_int16_t averageEventSize, size_t delta, u_int16_t sourceID, char* metaBlock, size_t metaLength)
{
	u_int16_t* sizePointer = m_randomFragmentSizes + m_randomFramgentsOffset;
	//Generate event fragments until we run out of space
	MEPFragmentHeader* header = (MEPFragmentHeader*)(metaBlock);
	int16_t newSize;
	size_t dataSize = 0;
	size_t metaSize = sizeof(MEPFragmentHeader);

	size_t totalSize = 0;
	size_t numEvents = 0;

	while(true)
	{
		//header->offsets[i] = totalSize;
		//newSize = averageEventSize;
		newSize = ((*(sizePointer++)) % (delta * 2)) - delta;
		newSize += averageEventSize;
		totalSize += newSize;
		//Wrap around
		if(++m_randomFramgentsOffset > m_numInts * 2)
		{
			m_randomFramgentsOffset = 0;
			sizePointer = m_randomFragmentSizes;
		}
		dataSize += newSize;
		metaSize += sizeof(unsigned short);

		if(dataSize > dataLength || metaSize > metaLength)
		{
			dataSize -= newSize;
			metaSize -= sizeof(unsigned short);
			break;
		}
		header->offsets[numEvents++] = newSize;

	}
	header->nEvents = numEvents;
	header->sourceID = sourceID;
	header->type = 1;
	header->len = dataSize;
	header->eventID = 0;
}

EventFrameGenerator::~EventFrameGenerator()
{
	free(m_randomFragmentSizes);
}
