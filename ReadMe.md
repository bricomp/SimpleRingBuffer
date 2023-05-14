#### ***SimpleRingBuffer***  

*Note: This library has been renamed from UniversalRingBuffer.*

​	This library is designed to offer a Ring buffer control aka **Rolodex**.
Although other libraries exist they are usually limited to storing standard numeric
types, eg uint32_t, int etc. With this library you can use any type you wish, create 
your own type in a struct and use that. No problem.

​	The library manages the index into the array/storage structure. It is NOT limited to 
storage in an array in memory but could be used to store data in EEPROM, SD card or
any other storage medium.
Note that this library does NOT store any data but manages the returning of an index
into an **Array** or **Data Structure** to indicate where to store or retrieve the data.

If you examine the Example **TestRingBuffer** I have used an array of char, just because
it's easy to track the addition of ASCII characters to demonstrate usage.

​	You start off by defining your storage medium. As I said before it can be used for 
any storage medium but for the purposes of describing it's usage I will use arrays of char
by way of examples.

​	Then create an instance of the **RingBuffer** passing the Maximum size of the array.
By maximum size I mean maximum index, NOT the storage size i.e. `data[ maximumSize]`.

###### AddDataToBuffer()

​	Ok, that's it we can now start storing data into our "**Rolodex**" type storage medium.
To add data just use **AddDataToBuffer()**. This function returns the index of the array
where the data is to be stored. So usage is **buffer[ cBuf.AddDataToBuffe() ] = dataChar**.

​	You can keep on adding data to your hearts content, storing many more items than the
original array could accommodate. Bear in mind that once you have exceeded the size of the
original array that the data will roll over and overwrite the original data. The array index
returned by **AddDataToBuffer()** will always be a legal array location. Think of this like
having completely filled your Rolodex and have to remove a card to add new information.
The library works in a LIFO manner.

###### ReadFromHead(), ReadFromTail()

​	Having stored the data you will want to do something with it. Retrieval of the data starts
by using the functions either **ReadFromHead()** or **ReadFromTail()**. Head represents the 
last entered data and tail represents the first entered data. **ReadFromHead()** and 
**ReadFromTail()** both return an index into the array of data. they can be used like:-
`Serial.print( buffer[ cBuf.ReadFromHead() ] );` where the data is returned from the array
and printed. 

###### ReadNext(),  ReadNext(bool changeDirection)

​	In order to continue retrieving data from the store (array) we now use the function
**ReadNext().** This returns the next array index in the sequence and is used in the
same manner as the previously described functions **ReadFromHead()** and **ReadFromTail()**, 
those having set the direction for subsequent **ReadNext()**s. As you can imagine it would
be possible to read past head or tail which would give rise to an erroneous condition.
To ensure that this does not happen the boolean variable **badCommand** is set to true
and the index returned by **ReadNext()** is NOT incremented or decremented, i.e. the 
previous value is returned.

​	Note that as of vs 1.10 **ReadNext** can have the optional option to change read direction.
 **ReadNext()** returns the next data index in the original set direction whilst **ReadNext(true)**
 changes the direction and returns the previous index.

The **badCommand** is reset to false by using either **ReadFromHead()** or **ReadFromTail()** again.

###### MaxBufferSize(),  currentBufferLength()

​	In order to help with data management there are two functions which return array information. 
these are **MaxBufferSize()**, which effectively returns the original maximum array index passed
when the instance of **RingBuffer** was created, and **currentBufferLength()** which returns the 
number of entries into the array. If the system has started to roll over then **currentBufferLength()**
will be the same as **MaxBufferSize()**. 

Obviously before any data has been added **currentBufferLength()** will be zero.

###### SaveRBInfo(),  RestoreRBInfo()

​	Should you wish to power down your system, but not lose your Ring Buffer database, then you can use the function **SaveRBInfo()**. First start by saving your Array Data away then get the **RingBuffer**
internal data with `RingBuffer::infoType rbData = SaverBData();` then save that rbData away.

​	To recover your Ring Buffer restore your array data and then read in your rbData and then
use the function **RestoreRBInfo( rbData )** to put the data back. This function attempts to check
the validity of the data and return bool if the data looks legal. If false is returned the data is
not valid and the **RingBuffer** internal variables will NOT have been updated.