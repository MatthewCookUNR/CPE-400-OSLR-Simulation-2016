#include "../include/olsr.h"
#include <iostream>

using namespace std;

OLSR::OLSR()
{
	pushNodes(12);
        network[0]->addOneHopNeighbor(network[1]);
        network[0]->addOneHopNeighbor(network[3]);
        network[0]->addOneHopNeighbor(network[5]);
        network[1]->addOneHopNeighbor(network[0]);
        network[1]->addOneHopNeighbor(network[2]);
        network[1]->addOneHopNeighbor(network[9]);
        network[2]->addOneHopNeighbor(network[1]);
        network[2]->addOneHopNeighbor(network[3]);
        network[3]->addOneHopNeighbor(network[0]);
        network[3]->addOneHopNeighbor(network[2]);
        network[3]->addOneHopNeighbor(network[4]);
        network[4]->addOneHopNeighbor(network[3]);
        network[4]->addOneHopNeighbor(network[7]);
        network[5]->addOneHopNeighbor(network[0]);
        network[5]->addOneHopNeighbor(network[6]);
        network[6]->addOneHopNeighbor(network[5]);
        network[6]->addOneHopNeighbor(network[7]);
        network[7]->addOneHopNeighbor(network[4]);
        network[7]->addOneHopNeighbor(network[6]);
        network[7]->addOneHopNeighbor(network[8]);
        network[8]->addOneHopNeighbor(network[7]);
        network[9]->addOneHopNeighbor(network[1]);
        network[9]->addOneHopNeighbor(network[10]);
        network[10]->addOneHopNeighbor(network[9]);
        network[10]->addOneHopNeighbor(network[11]);
        network[11]->addOneHopNeighbor(network[10]);

}

//Deletes vector object and frees the memory allocated
OLSR::~OLSR()
{
	network.clear();
	network.shrink_to_fit();
}

//Adds nodes to the network
void OLSR::pushNodes(int num)
{
	network.resize(num);
    for(int i = 0; i < num; i++)
    {
        network[i] = new Node(i);
    }
}

//Returns the current number of nodes in the network
int OLSR::getNumOfNodes()
{
	return network.size();
}

// Prints out the current power of each node in the network
void OLSR::checkNetworkPower()
{
	cout << "Remaining energy in the network" << endl;
	for(int i = 0; i < getNumOfNodes(); i++ )
	{
		cout << "Node " << network[i]->getNodeID() << ": " << network[i]->getEnergy() << endl;
	}
	cout << endl;
}

//Sets up 2 Hop Neighbor Table
void OLSR::broadcastHello(Node* node)
{
    for(int i = 0; i < node->getOneHopNeighborNum(); i++)	
    {
        Node *currentNeighbor = node -> getOneHopNeighbor(i);
        for(int j = 0; j < currentNeighbor -> getOneHopNeighborNum(); j++) 
        {
			for(int k = 0; k < node -> getOneHopNeighborNum(); k++)
            		{
            			if(node == currentNeighbor -> getOneHopNeighbor(j)) 
                		{
					break;
                		}
				else if((currentNeighbor -> getOneHopNeighbor(j) == node -> getOneHopNeighbor(k)))
				{
					break;
				}
				else if( node->inTwoHopTable(currentNeighbor -> getOneHopNeighbor(j)))
				{
					break;
				}
                		else
				{
					node -> addTwoHopNeighbor(currentNeighbor -> getOneHopNeighbor(j));
					break;
				}
    			}
	}
    }
	
}

// Get Node
Node* OLSR::getNode(int index)
{
    return network[index];
}

// Determine which nodes need to be MPRs and set them
void OLSR::topologyControl()
{
    for(int i = 0; i < getNumOfNodes(); i++)
    {
        if(network[i]->getOneHopNeighborNum() == 1) // check for isolated nodes
        {
            network[i]->getOneHopNeighbor(0)->setMPR(true);
        }
    }

    int counter = 4;    // number of connections to check for
    while(counter > 0)
    {
        for(unsigned int i = 0; i < network.size(); i++) // for each node in the network
        {
            if(network[i]->getOneHopNeighborNum() == counter)
            {
                for(int j = 0; j < network[i]->getOneHopNeighborNum(); j++) // for each one-hop neighbor
                {
                    if((!network[i]->getOneHopNeighbor(j)->neighboringMPR()))   // if neighbor doesn't have neighboring MPR
                    {
                        network[i]->setMPR(true);   // designate as MPR
						break;
                    }
                }
	            if(!network[i]->getMPR())
		        {
			        for( int k = 0; k < network[i]->getTwoHopNeighborNum(); k++ )   // for each two-hop neighbor
			        {
				        if((!network[i]->getTwoHopNeighbor(k)->neighboringMPR()))   // if two-hop neighbor doesn't have neighboring MPR
				        {
		                    network[i]->setMPR(true);   // designate as MPR
				            break;
				        }
			        }
		        }
            }
        }
        counter--;
    }

}

// Finds and sets a route in the routing table from one node to another
bool OLSR::findRoute(Node* origin, Node* prev, Node* src, Node* dest, int seqNum)
{
	static Route routeBuild;
    bool found = false;

	if(src->isOneHopNeighbor(dest))
	{
		routeBuild.setDestMPR(src);
		routeBuild.setDestAddress(dest);
        routeBuild.setMPRSequence(seqNum);
        for(int i = 0; i < origin->getTableSize(); i++)
        {
            if(routeBuild.getDestMPR() == origin->getRoute(i).getDestMPR() && routeBuild.getDestAddress() == origin->getRoute(i).getDestAddress())
            {
                if(routeBuild.getMPRSequence() >= origin->getRoute(i).getMPRSequence())
                {
                    return false;
                }
                if(routeBuild.getMPRSequence() < origin->getRoute(i).getMPRSequence())
                {
                    origin->removeRoute(i);
                }
            }
        }
    origin->pushRoute(routeBuild);		
	}
	else
	{
		for(int i = 0; i < src->getOneHopNeighborNum(); i++)
		{
			if(src->getOneHopNeighbor(i)->getMPR() && src->getOneHopNeighbor(i) != prev && src->getOneHopNeighbor(i) != origin)
			{
                if(found)
                {
                    seqNum--;
                }
                seqNum++;
                found = true;
				findRoute(origin, src, src->getOneHopNeighbor(i), dest, seqNum);
			}
		}
	}
    return true;
}

// Prints out the routing table
void OLSR::printRoutingTable()
{
    for(unsigned int i = 0; i < network.size(); i++)
    {
        cout << "Node " << network[i]->getNodeID() << " RoutingTable" << endl;
        for(int j = 0; j < network[i]->getTableSize(); j++)
        {
            cout  << "Dest Address - " << network[i]->getRoute(j).getDestAddress()->getNodeID() << "| DestMPR: " << network[i]->getRoute(j).getDestMPR()->getNodeID() << "| MPRSeq: " << network[i]->getRoute(j).getMPRSequence() << endl;  
        }
    }
}

// Send a packet from a source node to a destination (no energy constraint)            
bool OLSR::sendPacket(int srcID, int destID)
{
     Node* src = network[srcID];
    Node* dest = network[destID];
    Node* destMPR;
    Route currentRoute;
    Route fastestRoute;    
    fastestRoute.setMPRSequence(35);
    bool valid = false;
    for(int i = 0; i < src->getTableSize(); i++)
    {
        if(dest == src->getRoute(i).getDestAddress())
        {
            valid = true;
        }
    }


    if( src -> isOneHopNeighbor(dest) )
    {
        cout << "Node " << srcID << " to " << destID << endl;
        src -> losePower();
        dest -> losePower();
        return true;
    }
    for(int i = 0; i < src->getTableSize(); i++)
    {
        currentRoute = src->getRoute(i);
        if( currentRoute.getDestAddress() == dest && fastestRoute.getMPRSequence() > currentRoute.getMPRSequence() )
        {
            fastestRoute = currentRoute;
        }
    }
    destMPR = fastestRoute.getDestMPR();
    dest -> losePower();
    sendPacket(srcID, destMPR->getNodeID());
    cout << "Node " << destMPR->getNodeID() << " to " << destID << endl;


    checkNodes();
    return valid;

}

// Send a packet from a source node to a destination (with energy constraint)
bool OLSR::sendPacketEnergy(int srcID, int destID)
{
    Node* src = network[srcID];
    Node* dest = network[destID];
    Node* destMPR;
    Route currentRoute;
    Route fastestRoute;    
    fastestRoute.setMPRSequence(35);
    int fastRoute = 0;
    bool valid = false;


    for(int i = 0; i < src->getTableSize(); i++)
    {
        if(dest == src->getRoute(i).getDestAddress())
        {
            valid = true;
        }
    }


    if( src -> isOneHopNeighbor(dest) )
    {
        cout << "Node " << srcID << " to " << destID << endl;
        src -> losePower();
        dest -> losePower();
        return true;
    }
    for(int i = 0; i < src->getTableSize(); i++)
    {
            currentRoute = src->getRoute(i);
            for( int p = 0; p < src->getTableSize(); p++ )
            {
                currentRoute = src->getRoute(p);
                if( currentRoute.getDestAddress() == dest && fastestRoute.getMPRSequence() > currentRoute.getMPRSequence() )
                {
                        fastestRoute = currentRoute;
                        fastRoute = p;
                }
            }
            destMPR = fastestRoute.getDestMPR();
            if(destMPR->getEnergy() < 50)
            {
                vector<Route> tempTable = src->getRoutingTable();
                tempTable.erase(tempTable.begin() + fastRoute);
                for(unsigned int j = 0; j < tempTable.size(); j++)
                {
                    if(tempTable[j].getDestAddress() == dest)
                    {
                        fastestRoute = tempTable[j];
                        destMPR = fastestRoute.getDestMPR();
                    }
                }
            }
            dest -> losePower();
            sendPacket(srcID, destMPR->getNodeID());
            cout << "Node " << destMPR->getNodeID() << " to " << destID << endl;
            break;
    }
    checkNodes();
    return valid;

}

// Checks to see if any nodes have died and handles them accordingly
bool OLSR::checkNodes()
{
    for(int i = 0; i < getNumOfNodes(); i++)
    {
        if(network[i]->getEnergy() <= 0)
        {
            for(int j = 0; j < getNumOfNodes(); j++)
            {
                if(network[j]->isOneHopNeighbor(network[i]))
                {
                    network[j]->removeOneHopNeighbor(network[i]);
                }
                network[j]->clearTwoHop();
                network[j]->clearRoutingTable();
            }
            network.erase(network.begin() + i);
            
            return true;
        }
    }
    return false;
}
