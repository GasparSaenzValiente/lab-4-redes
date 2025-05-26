#ifndef NET
#define NET

#include <string.h>
#include <omnetpp.h>
#include <packet_m.h>
#include <distanceVector_m.h>
#include <vector>

using namespace omnetpp;

class Net: public cSimpleModule {
private:
    cOutVector delay;
    cOutVector hopCount;

    int id;
    struct RoutingEntry {
        int destination;
        int cost;
        int nextNode;
    };
    std::vector<RoutingEntry> routingTable;
    int interfaces;

public:
    Net();
    virtual ~Net();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
    virtual void sendTable();
    virtual void updateTable(DistanceVector * dv);
};

Define_Module(Net);

#endif /* NET */

Net::Net() {
}

Net::~Net() {
}

void Net::initialize() {
    delay.setName("Demora de entrega");
    hopCount.setName("Numero de saltos");
    this->id = this->getParentModule()->getIndex();
    this->interfaces = getParentModule()->par("interfaces").intValue();

    // esta linea es como un for inicializando las #interfaces entrys de la tabla en -1, -1.
    routingTable.resize(interfaces, RoutingEntry{-1, -1, -1});
}

void Net::finish() {
}

// func para enviar la distance table a nodos vecinos
void Net::sendTable(){
    DistanceVector * dv = new DistanceVector("dv");
    dv->setSenderId(id);

    int numNodes = routingTable.size();
    dv->setEntriesArraySize(numNodes);

    for(int i = 0; i < numNodes; i++){
        DistanceEntry dEntry;
        dEntry.cost = routingTable[i].cost;
        dEntry.destination = routingTable[i].destination;

        dv->setEntries(i, dEntry);
    }

   for(int i = 0; i < interfaces; i++){
        send(dv, "toLnk$o", i);
    }
}

// func para actualizar la distance table cuando me llega una
void Net::updateTable(DistanceVector * dv){

}




void Net::handleMessage(cMessage *msg) {

    // All msg (events) on net are packets
    Packet *pkt = (Packet *) msg;

    // If this node is the final destination, send to App
    if (pkt->getDestination() == id) {
        simtime_t delayTime = simTime() - pkt->getCreationTime();
        EV<< "Delay = " <<delayTime<<endl;
        delay.record(delayTime);
        hopCount.record(pkt->getHopCount());
        send(msg, "toApp$o");
    }
    // If not, forward the packet to some else... to who?
    else {
        // We send to link interface #0, which is the
        // one connected to the clockwise side of the ring
        // Is this the best choice? are there others?

        pkt->setHopCount(pkt->getHopCount() + 1);

        msg = (cMessage *) pkt;
        send(msg, "toLnk$o", 0);
    }
}
