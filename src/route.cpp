#include "../include/node.h"
#include <iostream>

/*------------------------------------------------
Constructor for route class
--------------------------------------------------*/
Route::Route()
{
	destAddress = NULL;
	destMPR = NULL;
	MPRSequence = 0;
}

/*------------------------------------------------
Destructor for route class
--------------------------------------------------*/
Route::~Route()
{
    destAddress = NULL;
    destMPR = NULL;
    MPRSequence = 0;
}

/*------------------------------------------------
Sets the destination address of the route
--------------------------------------------------*/
void Route::setDestAddress(Node* address)
{
	destAddress = address;
}

/*------------------------------------------------
Sets the destination MPR of the route
--------------------------------------------------*/
void Route::setDestMPR( Node* nodeMPR)
{
	destMPR = nodeMPR;
}

/*------------------------------------------------
Sets the MPRSequence of the route
--------------------------------------------------*/
void Route::setMPRSequence( int sequenceNum )
{
	MPRSequence = sequenceNum;
}

/*------------------------------------------------
Returns the destination address of the route
--------------------------------------------------*/
Node* Route::getDestAddress()
{
	return destAddress;
}

/*------------------------------------------------
Returns the destination MPR of the route
--------------------------------------------------*/
Node* Route::getDestMPR()
{
	return destMPR;
}

/*------------------------------------------------
Returns the MPR sequence of the route
--------------------------------------------------*/
int Route::getMPRSequence()
{
	return MPRSequence;
}
