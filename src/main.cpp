#include "../include/olsr.h"
#include <iostream>

using namespace std;

void test(OLSR *network);
void testEnergy(OLSR *network);

/*------------------------------------------------
Handles running the simulation
--------------------------------------------------*/
int main()
{
    int testNum;
    cout << "Select Test" << endl;
    cout << "Enter 1 for Test" << endl;
    cout << "Enter 2 for TestEnergy" << endl;
    cout << "Enter 3 to Print Routing Table" << endl;
    cin >> testNum;

	// Initialize network
    OLSR *myNetwork = new OLSR();

    // Builds 2-Hop Neighbor Tables
    for(int p = 0; p < myNetwork->getNumOfNodes(); p++)
    {
        myNetwork->broadcastHello(myNetwork->getNode(p));
    }
	cout << "Broadcast finishes" << endl;
	
    myNetwork->topologyControl();
	cout << "Topology Control finishes" << endl;
    
    // Builds routing table
    for(int i = 0; i < myNetwork->getNumOfNodes(); i++)
    {
        for(int j = 0; j < myNetwork->getNumOfNodes(); j++)
        {
            if(myNetwork->getNode(j) != myNetwork->getNode(i) && !(myNetwork->getNode(i)->isOneHopNeighbor(myNetwork->getNode(j))))
		    {
				cout << "find route from " << i << " to " << j << endl;
			    myNetwork->findRoute(myNetwork->getNode(i), NULL, myNetwork->getNode(i), myNetwork->getNode(j), 0);

		    }
        }
    }

	//Runs simulation without energy optimization
	//in consideration. 
    if(testNum == 1)
    {
		test(myNetwork);
    }
	
	//Runs simulation with energy optimization
	//in consideration. 
    if(testNum == 2)
    {
		testEnergy(myNetwork);
    }
	
	//Prints routing table
    if(testNum == 3)
    {
		myNetwork->printRoutingTable();
    }
   
    
    return 0;
}

/*------------------------------------------------
Simulates sending packets across the network and
details remaining power of the nodes
--------------------------------------------------*/
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

/*------------------------------------------------
Simulates (with energy constraint) sending packets 
across the network and details remaining power of 
the nodes
--------------------------------------------------*/
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
