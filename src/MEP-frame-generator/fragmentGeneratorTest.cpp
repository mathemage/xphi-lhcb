/* ========================================

   * File Name : fragmentGeneratorTest.cpp

   * Creation Date : 24-08-2015

   * Last Modified : Mon 24 Aug 2015 04:53:24 PM CEST

   * Created By : Rainer Schwemmer <rschwemm@cern.ch>
   
   * Modified by: Karel Ha <mathemage@gmail.com>

   ==========================================*/

#define SFMT_MEXP 19937

#include "eventFrameGenerator.h"

#define DATA_SIZE 1024*1024*4
#define AVERAGE_EVENT_SIZE 200
#define EVENT_SIZE_DELTA 20
#define MEP_FACTOR 100

#define RND_POOL_SIZE 1024*1024*100
//#define RND_POOL_SIZE 10

void parseDataBlock(char* data, size_t len) {
	FrameHeader* frameHeader = (FrameHeader*)data;

	char* dataPointer = data + sizeof(FrameHeader);

	MEPFragmentHeader* header;

	float averageEventSize = 0;
	unsigned int numEvents = 0;

	for(u_int32_t i = 0; i < frameHeader->numMEPs; ++i) {
		header = (MEPFragmentHeader*)dataPointer;

		for(int j = 0; j < header->nEvents; ++j) {
			printf("%d\n", header->offsets[j]);
			averageEventSize += header->offsets[j];
		}
		numEvents += header->nEvents;
	}

	printf("Average event size: %f\n", averageEventSize / numEvents);
}

int main(void) {
	EventFrameGenerator* gen = new EventFrameGenerator(RND_POOL_SIZE, 0);

	char* data = (char*)malloc(DATA_SIZE);
	while (true) {
		gen->fillMemoryBlock(data, DATA_SIZE, AVERAGE_EVENT_SIZE, EVENT_SIZE_DELTA, MEP_FACTOR, 0xFFEE);
  }

	parseDataBlock(data, DATA_SIZE);
}

