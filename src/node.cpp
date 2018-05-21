#include "../include/node.h"
#include <iostream>

/*------------------------------------------------
Constructor for node class
--------------------------------------------------*/ 
Node::Node(int ID)
{
	nodeID = ID;
    energy = 100;
	MPR = false;
    oneHopNeighbor.clear();
}

/*------------------------------------------------
Deletes vector objects and frees the memory 
allocated
--------------------------------------------------*/
Node::~Node()
{
	for(unsigned int i = 0; i < (oneHopNeighbor.size()); i++)
	{
		oneHopNeighbor[i] = NULL;
	}
	for(unsigned int j = 0; j < (twoHopNeighbor.size()); j++)
	{
		twoHopNeighbor[j] = NULL;
	}
	oneHopNeighbor.clear();
	oneHopNeighbor.shrink_to_fit();
	twoHopNeighbor.clear();
	twoHopNeighbor.shrink_to_fit();
}

/*------------------------------------------------
Adds a node to the current node's one hop neighbor 
table
--------------------------------------------------*/
void Node::addOneHopNeighbor(Node* neighbor)
{
    oneHopNeighbor.push_back(neighbor);
}

/*------------------------------------------------
Retrieves the size of the one hop neighbor table
--------------------------------------------------*/
int Node::getOneHopNeighborNum()
{
    return oneHopNeighbor.size();
}

/*------------------------------------------------
Returns the desired node in the one hop neighbor 
table
--------------------------------------------------*/
Node* Node::getOneHopNeighbor(int index)
{
    return oneHopNeighbor[index];
}

/*------------------------------------------------
Returns the entire one hop neighbor table
--------------------------------------------------*/
vector<Node*> Node::getOneHopNeighbors()
{
    return oneHopNeighbor;
}

/*------------------------------------------------
Checks if the node is within the two hop neighbor
table of the given node
--------------------------------------------------*/
bool Node::inTwoHopTable(Node* neighbor)
{
	for(int i = 0; i < getTwoHopNeighborNum(); i++)
	{
		if(twoHopNeighbor[i] == neighbor)
		{
			return true;
		}
	}
	return false;
}
/*------------------------------------------------
Adds a node to the two hop neighbor table
--------------------------------------------------*/

void Node::addTwoHopNeighbor(Node* neighbor)
{
    twoHopNeighbor.push_back(neighbor);
}

/*------------------------------------------------
Returns the size of the two hop neighbor table
--------------------------------------------------*/
int Node::getTwoHopNeighborNum()
{
    return twoHopNeighbor.size();
}

/*------------------------------------------------
Returns the desired node in the two hop neighbor table
--------------------------------------------------*/
Node* Node::getTwoHopNeighbor(int index)
{
    return twoHopNeighbor[index];
}

/*------------------------------------------------
Returns the entire two hop neighbor table
--------------------------------------------------*/
vector<Node*> Node::getTwoHopNeighbors()
{
    return twoHopNeighbor;
}

/*------------------------------------------------
Returns the id of the node
--------------------------------------------------*/
int Node::getNodeID()
{
	return nodeID;
}

/*------------------------------------------------
Sets the MPR value of the node
--------------------------------------------------*/
void Node::setMPR(bool flag)
{
    MPR = flag;
}

/*------------------------------------------------
Returns the MPR value of the node
--------------------------------------------------*/
bool Node::getMPR()
{
    return MPR;
}

/*------------------------------------------------
Checks if the node has a neighboring MPR
--------------------------------------------------*/
bool Node::neighboringMPR()
{
    bool neighboringMPR = false;
    for(int i = 0; i < getOneHopNeighborNum(); i++)
    {
        if(oneHopNeighbor[i]->getMPR())
        {
            neighboringMPR = true;
        }
    }
    return neighboringMPR;
}

/*------------------------------------------------
Checks if argument is a  one hop neighbor of the current node
--------------------------------------------------*/
bool Node::isOneHopNeighbor(Node* node)
{
    for(unsigned int i = 0; i < oneHopNeighbor.size(); i++)
    {
        if(node == oneHopNeighbor[i])
        {
            return true;
        }
    }
    return false;
}

/*------------------------------------------------
Pushes a route onto the routing table
--------------------------------------------------*/
void Node::pushRoute(Route route)
{
    routingTable.push_back(route);
}

/*------------------------------------------------
Returns the desired route in the routing table
--------------------------------------------------*/
Route Node::getRoute(int routeNum)
{
    return routingTable[routeNum];
}

/*------------------------------------------------
Returns the size of the routing table
--------------------------------------------------*/
int Node::getTableSize()
{
    return routingTable.size();
}

/*------------------------------------------------
Returns the routing table of the node
--------------------------------------------------*/
vector<Route> Node::getRoutingTable()
{
	return routingTable;
}

/*------------------------------------------------
Returns the energy of the node
--------------------------------------------------*/
int Node::getEnergy()
{
	return energy;
}

/*------------------------------------------------
Decrements the energy of the node
--------------------------------------------------*/
void Node::losePower()
{
	energy = energy -10;
}

/*------------------------------------------------
Removes the desired route from the routing table
--------------------------------------------------*/
void Node::removeRoute(int index)
{
    routingTable.erase(routingTable.begin() + index);
}

/*------------------------------------------------
Removes the desired node from the one hop neighbor table
--------------------------------------------------*/
void Node::removeOneHopNeighbor(Node *node)
{
    for(unsigned int i = 0; i < oneHopNeighbor.size(); i++)
    {
        if(node == oneHopNeighbor[i])
        {
            oneHopNeighbor.erase(oneHopNeighbor.begin() + i);
        }
    }
}

/*------------------------------------------------
Clears the two hop neighbor table
--------------------------------------------------*/
void Node::clearTwoHop()
{
    twoHopNeighbor.clear();
    twoHopNeighbor.shrink_to_fit();
}

/*------------------------------------------------
Clears the routing table
--------------------------------------------------*/
void Node::clearRoutingTable()
{
    routingTable.clear();
    routingTable.shrink_to_fit();
}
