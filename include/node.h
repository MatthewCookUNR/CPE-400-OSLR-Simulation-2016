#ifndef NODE_H
#define NODE_H

#include <vector>

using namespace std;

class Node;

/*------------------------------------------------
Route class contains the properties of a route
needed for the simulation
--------------------------------------------------*/ 
class Route
{
	public:
		Route();
		~Route();

		void setDestAddress(Node* address);
		void setDestMPR( Node* nodeMPR);
		void setMPRSequence( int sequenceNum );
		Node* getDestAddress();
		Node* getDestMPR();
		int getMPRSequence();

	private:
	    Node* destAddress;
		Node* destMPR;
		int MPRSequence;
};

/*------------------------------------------------
Node class contains the the necessary functionality
of a node to run the simulation.
--------------------------------------------------*/ 
class Node
{
    public:
        Node(int ID);
        ~Node();
		
	    int getNodeID();
	    bool inTwoHopTable(Node* neighbor);
        bool isOneHopNeighbor(Node* node);
        vector<Node*> getOneHopNeighbors();
        vector<Node*> getTwoHopNeighbors();
        void addOneHopNeighbor(Node* neighbor);
        Node* getOneHopNeighbor(int index);
        void removeOneHopNeighbor(Node *node);
        int getOneHopNeighborNum();
	    void addTwoHopNeighbor(Node* neighbor);
        Node* getTwoHopNeighbor(int index);
        int getTwoHopNeighborNum();
        void setMPR(bool flag);
        bool getMPR();
        bool neighboringMPR();
	    void pushRoute(Route route);
		vector<Route> getRoutingTable();
        Route getRoute(int routeNum);
        int getTableSize();
		int getEnergy();
		void losePower();
        void removeRoute(int index);
        void clearTwoHop();
        void clearRoutingTable();

    private:
        int energy;
	    int nodeID;
        vector<Node*> oneHopNeighbor;
        vector<Node*> twoHopNeighbor;
        bool MPR;
	    vector<Route> routingTable;
};



#endif /* NODE_H */
