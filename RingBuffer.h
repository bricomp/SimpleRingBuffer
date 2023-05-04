#pragma once
/*
 The MIT License (MIT)
  Copyright (c) 2023 Robert E Bridges
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

  Revision	   Date		Author				   Description
  1.00		04/05/2023	Robert E Bridges	- Initial release

  This library is designed to offer a Ring buffer control aka Roladex.
  Although other libraries exist they are usually limited to storing standard numeric
  types, eg uint32_t, int etc. With this library you can use any type you wish, create 
  your own type in a struct and use that. No problem.

  The library manages the index into the array/storage structure. It is NOT limited to 
  storage in an array in memory but could be used to store data in EEPROM, SD card or
  any other storage medium.

  If you examine the Example "TestRingBuffer" I have used an array of char, just because
  it's easy to track the addition of ASCII characters to demonstrate useage.

  You start off by defining your storage medium. As I said before it can be used for 
  any storage medium but for the purposes of describing it's useage I will use arrays 
  by way of examples.

  Then create an instance of the "RingBuffer" passing the Maximum size of the array.
  By maximum size I mean maximum index, NOT the storage size i.e. data[ maximumSize].

  Ok, that's it we can now start storing data into our "Rolodex" type storage medium.
  To add data just use "AddDataToBuffer()". This function returns the index of the array
  where the data is to be stored. So useage is buffer[ cBuf.AddDataToBuffe() ] = dataChar.

  You can keep on adding data to your hearts content, storing many more items than the
  original array could accomodate. Bear in mind that once you have exceeded the size of the
  original array that the data will roll over and overwrite the original data. The array index
  returned by "AddDataToBuffer()" will always be a legal array location. Think of this like
  having completely filled your Rolodex and have to remove a card to add new information.
  The library works in a LIFO manner.

  Having stored the data you will want to do something with it. Retrieval of the data starts
  by using the functions either "ReadFromHead()" or "ReadFromTail()". Head represents the 
  last entered data and tail represents the first entered data. "ReadFromHead()" and 
  "ReadFromTail()" both return an index into the array of data. they can be used like:-
  Serial.print( buffer[ cBuf.ReadFromHead() ] ); where the data is returned from the array
  and printed. 
  
  In order to continue retrieving data from the store (array) we now use the function
  "ReadNext()". This returns the next array index in the sequence and is used in the
  same manner as the previously described functions "ReadFromHead()" and "ReadFromTail()", 
  those having set the direction for subsequent "ReadNext()"s. As you can imagine it would
  be possible to read past head or tail which would give rise to an erroneous condition.
  To ensure that this does not happen the boolean variable "BadCommand" is set to true
  and the index returned by "ReadNext()" is NOT incremented or decremented, i.e. the 
  previous value is returned.

  The boolean variable is reset to false by using either "ReadFromHead() or "ReadFromTail()" again.

  In order to help with data management there are two functions which return array information. 
  these are "MaxBufferSize()", which effectively returns the original maximum array index passed
  when the instance of RingBuffer was created, and "currentBufferLength()" which returns the 
  number of entries into the array. If the system has started to roll over then "currentBufferLength()"
  will be the same as "MaxBufferSize()". 
  
  Obviously before any data has been added "currentBufferLength()" will be zero.

  Should you wish to power down your system but not lose your Ring Buffer database then you can use
  the function "SaveRBInfo". First start by saving your ArrayData away then get the RingBuffer
  internal data with "RingBuffer::infoType rbData = SaveRBData();" then save that datya away.

  To recover your Ring Buffer restore your array data and then read in your cbData and then
  use the function "RestoreRBInfo( rbData )" to put the data back. This function attempts to check
  the validity of the data and return bool if the data looks legal. If false is returned the data is
  not valid and the RingBuffer internal variables will NOT have been updated.


*/
#include "Arduino.h"

class RingBuffer {

public:

	enum dirType {
		forward,
		backward
	};

	struct infoType {
		uint32_t h;
		uint32_t t;
		uint32_t bS;
		uint32_t lRL;
		dirType rD;
		uint32_t lRP;
		uint32_t lAL;
		bool	 c;
	};

	const char		revision[5] = "1.00";
	const uint32_t  revisionNum = 100;
	bool			badCommand;

	RingBuffer(uint32_t bufferSize);

	uint32_t AddDataToBuffer();
	uint32_t MaxBufferSize();
	uint32_t currentBufferLength();
	uint32_t ReadFromTail();
	uint32_t ReadFromHead();
	uint32_t ReadNext();

	infoType SaveRBInfo();
	bool	 RestoreRBInfo(infoType info);

private:

	uint32_t head;
	uint32_t tail;
	uint32_t bufSize;
	uint32_t lastReadLocn;
	dirType	 readDirection;
	uint32_t lastReadPos;
	uint32_t lastArrayLocn;
	bool	 circulating;
};


