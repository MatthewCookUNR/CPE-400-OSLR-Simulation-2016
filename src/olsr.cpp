#include "../include/olsr.h"
#include <iostream>

using namespace std;

/*------------------------------------------------
Creates the nodes needed for the network and 
connects them together.
--------------------------------------------------*/
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

/*------------------------------------------------
Deletes vector objects and frees the memory 
allocated
--------------------------------------------------*/
OLSR::~OLSR()
{
	network.clear();
	network.shrink_to_fit();
}

/*------------------------------------------------
Adds nodes to the network
--------------------------------------------------*/
void OLSR::pushNodes(int num)
{
	network.resize(num);
    for(int i = 0; i < num; i++)
    {
        network[i] = new Node(i);
    }
}

/*------------------------------------------------
Returns the current number of nodes in the network
--------------------------------------------------*/
int OLSR::getNumOfNodes()
{
	return network.size();
}

/*------------------------------------------------
Prints out the current power of each node in the 
network
--------------------------------------------------*/
void OLSR::checkNetworkPower()
{
	cout << "Remaining energy in the network" << endl;
	for(int i = 0; i < getNumOfNodes(); i++ )
	{
		cout << "Node " << network[i]->getNodeID() << ": " << network[i]->getEnergy() << endl;
	}
	cout << endl;
}

/*------------------------------------------------
Sets up 2 Hop Neighbor Table for the node given
--------------------------------------------------*/
void OLSR::broadcastHello(Node* node)
{
	//Loops runs for each 1-Hop neighbor of the given node
    for(int i = 0; i < node -> getOneHopNeighborNum(); i++)	
    {
        Node *currentNeighbor = node -> getOneHopNeighbor(i);
		
		//Loop runs for each 1-Hop neighbor of the current 1-Hop 
		//neighbor selected (neighbors of neighbors)
        for(int j = 0; j < currentNeighbor -> getOneHopNeighborNum(); j++) 
        {
			//Loops runs for each 1-Hop neighbor of the given node
			for(int k = 0; k < node -> getOneHopNeighborNum(); k++)
            {
				//Checks to see if current neighbor's neighbor is the
				//given node
            	if(node == currentNeighbor -> getOneHopNeighbor(j)) 
                {
					break;
                }
				
				//Checks if the current neighbor's neighbor is a 1-Hop
				//neighbor of the given node
				else if((currentNeighbor -> getOneHopNeighbor(j) == node -> getOneHopNeighbor(k)))
				{
					break;
				}
				
				//Checks if the current neighbor's neighbor is in the 2-Hop
				//neighbor table of the given node
				else if( node->inTwoHopTable(currentNeighbor -> getOneHopNeighbor(j)))
				{
					break;
				}
				
				//If the previous checks are all false, then add the current 
				//node the given node's 2-Hop neighbor table
                else
				{
					node -> addTwoHopNeighbor(currentNeighbor -> getOneHopNeighbor(j));
					break;
				}
    		}
		}
    }
	
}

/*------------------------------------------------
Gets a specified node in the network
--------------------------------------------------*/
Node* OLSR::getNode(int index)
{
    return network[index];
}

/*------------------------------------------------
Determine which nodes need to be MPRs and set them
as MPRs
--------------------------------------------------*/
void OLSR::topologyControl()
{
    for(int i = 0; i < getNumOfNodes(); i++)
    {
		//Checks if the node is isolated (only 1 neighbor)
        if(network[i]->getOneHopNeighborNum() == 1)
        {
			//Set that neighbor as a MPR
            network[i]->getOneHopNeighbor(0)->setMPR(true);
        }
    }

	// Number of connections to check for
    int counter = 4;  
	
    while(counter > 0)
    {
		//Loops for each node in the network
        for(unsigned int i = 0; i < network.size(); i++)
        {
            if(network[i]->getOneHopNeighborNum() == counter)
            {
				//Loops for each 1-hop neighbor of a node
                for(int j = 0; j < network[i]->getOneHopNeighborNum(); j++)
                {
					// Checks if neighbor doesn't have neighboring MPR
                    if((!network[i]->getOneHopNeighbor(j)->neighboringMPR()))   
                    {
						//Sets the node as a MPR
                        network[i]->setMPR(true);   
						break;
                    }
                }
	            if(!network[i]->getMPR())
		        {
					//Loops for each 2-hop neighbor
			        for( int k = 0; k < network[i]->getTwoHopNeighborNum(); k++ )
			        {
						// Checks if two-hop neighbor doesn't have neighboring MPR
				        if((!network[i]->getTwoHopNeighbor(k)->neighboringMPR()))
				        {
							//Sets the node as a MPR
		                    network[i]->setMPR(true);   
				            break;
				        }
			        }
		        }
            }
        }
        counter--;
    }

}

/*------------------------------------------------
Finds and sets a route in the routing table from 
one node to another
--------------------------------------------------*/
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
				//Checks to see if the new route is longer than one
				//currently in the node's routing table
                if(routeBuild.getMPRSequence() >= origin->getRoute(i).getMPRSequence())
                {
					//Returns false and doesn't add the longer route
                    return false;
                }
                else(routeBuild.getMPRSequence() < origin->getRoute(i).getMPRSequence())
                {
					//Removes the longer route
                    origin->removeRoute(i);
                }
            }
        }
		//Adds the new route to the routing table
		origin->pushRoute(routeBuild);		
	}
	else
	{
		for(int i = 0; i < src->getOneHopNeighborNum(); i++)
		{
			//Checks to see if neighbor is a MPR, is not the previous node, and not the origin node
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

/*------------------------------------------------
Prints out the routing table
--------------------------------------------------*/
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

/*------------------------------------------------
Send a packet from a source node to a destination 
(no energy constraint)
--------------------------------------------------*/            
bool OLSR::sendPacket(int srcID, int destID)
{
    Node* src = network[srcID];
    Node* dest = network[destID];
    Node* destMPR;
    Route currentRoute;
    Route fastestRoute;    
    fastestRoute.setMPRSequence(35);
    bool valid = false;
	
	//Loops through the node's routing table
    for(int i = 0; i < src->getTableSize(); i++)
    {
		//Checks if destination of the route is 
		//the destination of the packet
        if(dest == src->getRoute(i).getDestAddress())
        {
            valid = true;
        }
    }

	//Checks to see if node is a 1-hop neighbor
    if( src -> isOneHopNeighbor(dest) )
    {
		//Nodes lose power appropriately
        cout << "Node " << srcID << " to " << destID << endl;
        src -> losePower();
        dest -> losePower();
        return true;
    }
	
	//Loops through the src node's routing table
    for(int i = 0; i < src->getTableSize(); i++)
    {
        currentRoute = src->getRoute(i);
		
		//Checks if a faster route than the current route 
		//has been found
        if( currentRoute.getDestAddress() == dest && fastestRoute.getMPRSequence() > currentRoute.getMPRSequence() )
        {
            fastestRoute = currentRoute;
        }
    }
	
	//Get node that is the destination MPR
    destMPR = fastestRoute.getDestMPR();
    dest -> losePower();
    sendPacket(srcID, destMPR->getNodeID());
    cout << "Node " << destMPR->getNodeID() << " to " << destID << endl;

    checkNodes();
    return valid;

}

/*------------------------------------------------
Send a packet from a source node to a destination 
(with energy constraint)
--------------------------------------------------*/
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

	//Loops through the node's routing table
    for(int i = 0; i < src->getTableSize(); i++)
    {
		//Checks if destination of the route is 
		//the destination of the packet
        if(dest == src->getRoute(i).getDestAddress())
        {
            valid = true;
        }
    }

	//Checks to see if node is a 1-hop neighbor
    if( src -> isOneHopNeighbor(dest) )
    {
        cout << "Node " << srcID << " to " << destID << endl;
        src -> losePower();
        dest -> losePower();
        return true;
    }
	
	//Loops through the src node's routing table
    for(int i = 0; i < src->getTableSize(); i++)
    {
            currentRoute = src->getRoute(i);
            for( int p = 0; p < src->getTableSize(); p++ )
            {
                currentRoute = src->getRoute(p);
				
				//Checks if a faster route than the current route 
				//has been found
                if( currentRoute.getDestAddress() == dest && fastestRoute.getMPRSequence() > currentRoute.getMPRSequence() )
                {
                        fastestRoute = currentRoute;
                        fastRoute = p;
                }
            }
			
			//Get node that is the destination MPR
            destMPR = fastestRoute.getDestMPR();
			
			//Checks if the destination MPR's energy
			//is less than the threshold
            if(destMPR->getEnergy() < 50)
            {
                vector<Route> tempTable = src->getRoutingTable();
                tempTable.erase(tempTable.begin() + fastRoute);
				
				//Loops through routing table of the src node
                for(unsigned int j = 0; j < tempTable.size(); j++)
                {
					//Checks if another route is found to the 
					//destination exist
                    if(tempTable[j].getDestAddress() == dest)
                    {
						//Replace fastest route with alternative
						//route to conserve energy
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

/*------------------------------------------------
Checks to see if any nodes have died and handles 
them accordingly
--------------------------------------------------*/
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
