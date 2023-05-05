/*
 The MIT License (MIT)
  Copyright (c) 2022 Robert E Bridges
  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  On a personal note, if you develop an application or product using this library
  and make millions of dollars, I'm happy for you!
*/

/*
  Code by Robert E Bridges bob@bricomp-uk.com
*/


#include "RingBuffer.h"

// head always points to the next slot to use.

RingBuffer::RingBuffer(uint32_t bufferSize) {
	bufSize			= bufferSize;
	lastArrayLocn	= bufSize - 1;
	lastReadLocn	= 0;
	readDirection	= forward;
	circulating		= false;
	badCommand		= false;
	head			= 0;
	tail			= 0;
};

uint32_t RingBuffer::AddDataToBuffer() {

	uint32_t h;

	h = head;
	head++;
	if ((h == tail) && circulating) {
		tail++;
		if (tail > lastArrayLocn) tail = 0;
		lastReadLocn  = tail;
		readDirection = forward;
	}
	if (head > lastArrayLocn) {
		head = 0;
		circulating = true;
	}
	badCommand = false;
	return h;
};

uint32_t RingBuffer::MaxBufferSize() {   // Effectively size of array holding data
	return bufSize;
};

uint32_t RingBuffer::currentBufferLength() {
	if (circulating) return bufSize; else return (head);
};

uint32_t RingBuffer::ReadFromTail() {
	readDirection = forward;
	lastReadLocn = tail;
	if ((head == tail) && !circulating) // Haven't got any data yet
		badCommand = true;
	else
		badCommand = false;
	return lastReadLocn;
};

uint32_t RingBuffer::ReadFromHead() {
	badCommand = false;
	if (circulating) {
		if (head == 0) lastReadLocn = lastArrayLocn; else lastReadLocn = head - 1;
	}
	else {
		if (head == tail) {  // Haven't got any data yet
			lastReadLocn = 0;
			badCommand = true;
		}
		else {
			lastReadLocn = head - 1;
		}
	}
	readDirection = backward;
	return lastReadLocn;
};

uint32_t RingBuffer::ReadNext(bool changeDirection) {

	if (badCommand) return lastReadLocn;

	if (changeDirection){
		if (readDirection == forward) readDirection = backward; else readDirection = forward;
	}
	if (readDirection == forward) {  // from tail towards head
		lastReadLocn++;
		if (lastReadLocn > lastArrayLocn) lastReadLocn = 0;
		if (lastReadLocn == head) {
			if (lastReadLocn == 0) lastReadLocn = lastArrayLocn; else lastReadLocn--;
			badCommand = true;
		}
	}
	else {  // from head towards tail
		if (lastReadLocn == tail) {
			badCommand = true;
		}
		else {
			if (lastReadLocn > 0) lastReadLocn--; else lastReadLocn = lastArrayLocn;
		}
	}
	return lastReadLocn;
};

RingBuffer::infoType RingBuffer::SaveRBInfo() {
	infoType cbi;

	cbi.h   = head;
	cbi.t   = tail;
	cbi.bS  = bufSize;
	cbi.lRL = lastReadLocn;
	cbi.rD  = readDirection;
	cbi.lRP = lastReadPos;
	cbi.lAL = lastArrayLocn;
	cbi.c   = circulating;

	return cbi;
};

bool RingBuffer::RestoreRBInfo(infoType info) {
	if ((info.h < bufSize) 
			&& (info.t < bufSize) 
				&& (info.bS == bufSize) 
					&& (info.lRL < bufSize) 
						&& (info.lRP < bufSize) 
							&& (info.lAL == lastArrayLocn)) {
		head			= info.h;
		tail			= info.t;
		bufSize			= info.bS;
		lastReadLocn	= info.lRL;
		readDirection	= info.rD;
		lastReadPos		= info.lRP;
		lastArrayLocn	= info.lAL;
		circulating		= info.c;
		return true;
	} else return false;
};
