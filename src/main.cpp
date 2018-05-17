#include "../include/olsr.h"
#include <iostream>

using namespace std;

void update(OLSR *network);
void test(OLSR *network);
void testEnergy(OLSR *network);

int main()
{
    int testNum;
    cout << "Select Test" << endl;
    cout << "Enter 1 for Test" << endl;
    cout << "Enter 2 for TestEnergy" << endl;
    cout << "Enter 3 to Print Routing Table" << endl;
    cin >> testNum;

    OLSR *myNetwork = new OLSR();

    // Build 2-Hop Neighbor Tables
    for(int p = 0; p < myNetwork->getNumOfNodes(); p++)
    {
        myNetwork->broadcastHello(myNetwork->getNode(p));
    }
	
    myNetwork->topologyControl();
    
    // Build routing table
    for(int i = 0; i < myNetwork->getNumOfNodes(); i++)
    {
        for(int j = 0; j < myNetwork->getNumOfNodes(); j++)
        {
            if(myNetwork->getNode(j) != myNetwork->getNode(i) && !(myNetwork->getNode(i)->isOneHopNeighbor(myNetwork->getNode(j))))
		    {
			    myNetwork->findRoute(myNetwork->getNode(i), NULL, myNetwork->getNode(i), myNetwork->getNode(j), 0);
		    }
        }
    }

    if(testNum == 1)
    {
	test(myNetwork);
    }
    if(testNum == 2)
    {
	testEnergy(myNetwork);
    }
    if(testNum == 3)
    {
	myNetwork->printRoutingTable();
    }
   
    
    return 0;
}

void update(OLSR *network)
{
    for(int p = 0; p < network->getNumOfNodes(); p++)
    {
        network->broadcastHello(network->getNode(p));
    }
    network->topologyControl();

    for(int i = 0; i < network->getNumOfNodes(); i++)
    {
        for(int j = 0; j < network->getNumOfNodes(); j++)
        {
            if(network->getNode(j) != network->getNode(i) && !(network->getNode(i)->isOneHopNeighbor(network->getNode(j))))
		    {
			    network->findRoute(network->getNode(i), NULL, network->getNode(i), network->getNode(j), 0);
		    }
        }
    }
}

void test(OLSR *network)
{
    for(int i = 0; i < 3; i++)
    {
        network->sendPacket(1, 9);
        network->checkNetworkPower();
    }
    for(int i = 0; i < 7; i++)
    {
        network->sendPacket(0, 2);
        network->checkNetworkPower();
    }
}    

void testEnergy(OLSR *network)
{
    for(int i = 0; i < 3; i++)
    {
        network->sendPacketEnergy(1, 9);
        network->checkNetworkPower();
    }
    for(int i = 0; i < 7; i++)
    {
        network->sendPacketEnergy(0, 2);
        network->checkNetworkPower();
    }
}
