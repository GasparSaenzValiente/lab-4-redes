#ifndef NET
#define NET

#include <string.h>
#include <omnetpp.h>
#include <packet_m.h>
#include <distanceVector_m.h>
#include <map>

using namespace omnetpp;

class Net: public cSimpleModule {
private:
    cOutVector delay;
    cOutVector hopCount;

    int id;
    struct RoutingEntry {
        int cost;
        int nextNode;
    };
    std::map<int, RoutingEntry> routingTable;
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

    RoutingEntry defaultEntry = {-1, -1, -1};
    for (int i = 0; i < this->interfaces; i++){
        routingTable.insert({i, defaultEntry});
    }


void Net::finish() {
}

// func para enviar la distance table a nodos vecinos
void Net::sendTable(){
    DistanceVector * dv = new DistanceVector("dv");
    dv->setSenderId(id);

    int numNodes = routingTable.size();
    dv->setEntriesArraySize(numNodes);

    int i = 0;
    for (auto& entry : routingTable) {
        DistanceEntry distanceE;

        distanceE.destination = entry.first;
        distanceE.cost = entry.second.cost;

        dv->setEntries(i, distanceE);
        i++;
    }

   for(int i = 0; i < interfaces; i++){
        send(dv, "toLnk$o", i);
    }
}

// func para actualizar la distance table cuando me llega una
void Net::updateTable(DistanceVector * dv){
    int dv_size = dv->getEntriesArraySize();
    int senderID = dv->getSenderId();

    for (int i = 0; i < dv_size; i++){
        DistanceEntry distanceE = dv->getEntries(i);

        auto it = routingTable.find(distanceE.destination);
        if (it == routingTable.end()) // el nodo destinatario es nuevo{
            RoutingEntry newEntry;
            newEntry.cost = distanceE.cost + 1; // +1 por el salto de ir al noda que envio el dv
            newEntry.nextNode = senderID;
            routingTable[distanceE.destination] = newEntry;
        }
        else{ // el nodo destinatario esta en nuestra dtable, y hay que ver el minimo
            RoutingEntry existingEntry = it->second;

            // si el costo es menor hago cosas
            if (existingEntry.cost > distanceE.cost + 1){
                existingEntry.cost = distanceE.cost + 1;
                existingEntry.nextNode = senderID;
            }
            // si no lo dejo como esta, skip if
        }
    }
}




void Net::handleMessage(cMessage *msg) {

    // msg es un PACKET
    if (Packet *pkt = dynamic_cast<Packet*>(msg)){

         // If this node is the final destination, send to App
        if (pkt->getDestination() == id) {
            simtime_t delayTime = simTime() - pkt->getCreationTime();
            EV<< "Delay = " <<delayTime<<endl;
            delay.record(delayTime);
            hopCount.record(pkt->getHopCount());
            send(msg, "toApp$o");
        }
         // If not, forward the packet to some else
         // debemos chequear la tabla para saber para donde mandarlo
        else {
            pkt->setHopCount(pkt->getHopCount() + 1);

            msg = (cMessage *) pkt;
            send(msg, "toLnk$o", 0);
        }
    }

    // msg es un DistanceVector
    else if (DistanceVector *dv = dynamic_cast<DistanceVector*>(msg)) {
        updateTable(dv);
    }

    // deberia haber un tipo hello despues o algo asi para saber quienes son mis vecinos?
}
