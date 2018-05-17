#include "../include/node.h"
#include <iostream>

// constructor for route class
Route::Route()
{
	destAddress = NULL;
	destMPR = NULL;
	MPRSequence = 0;
}

// destructor for route class
Route::~Route()
{
    destAddress = NULL;
    destMPR = NULL;
    MPRSequence = 0;
}

// sets the destination address of the route
void Route::setDestAddress(Node* address)
{
	destAddress = address;
}

// sets the destination MPR of the route
void Route::setDestMPR( Node* nodeMPR)
{
	destMPR = nodeMPR;
}

// sets the MPRSequence of the route
void Route::setMPRSequence( int sequenceNum )
{
	MPRSequence = sequenceNum;
}

// returns the destination address of the route
Node* Route::getDestAddress()
{
	return destAddress;
}

// returns the destination MPR of the route
Node* Route::getDestMPR()
{
	return destMPR;
}

// returns the MPR sequence of the route
int Route::getMPRSequence()
{
	return MPRSequence;
}
