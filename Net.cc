#ifndef NET
#define NET

#include <string.h>
#include <omnetpp.h>
#include <packet_m.h>
#include <distanceVector_m.h>
#include <hello_m.h>


using namespace omnetpp;


const int INFINITY_COST = std::numeric_limits<int>::max();

struct RoutingEntry {
        int cost;
        int nextNode;
};


class Net: public cSimpleModule {
private:
    cOutVector delay;
    cOutVector hopCount;

    int id;
    std::map<int, RoutingEntry> routingTable; // mapea un nodo destino al nodo vecino que tengo que reenviar
    std::map<int, int> gateTable; // toma un nodo vecino y devuelve el gate por donde enviar
    int interfaces;

public:
    Net();
    virtual ~Net();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
    virtual void sendTable();
    virtual bool updateTable(DistanceVectorMsg * dv);
    virtual void handlePacket(Packet *pkt);
    virtual void handleDistanceVector(DistanceVectorMsg *dv);
    virtual void handleHello(Hello *helloPkt);
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
    this->interfaces = gateSize("toLnk$o");

    routingTable[this->id] = {0, this->id};
    gateTable[id] = -1;
    for(int i = 0; i < interfaces; i++){
        Hello * helloPkt = new Hello("helloMsg");
        helloPkt->setSenderID(id);
        send(helloPkt, "toLnk$o", i);
    }
}

void Net::finish() {
}


void Net::handleMessage(cMessage *msg) {
    // msg es un PACKET
    if (Packet *pkt = dynamic_cast<Packet*>(msg)){
        handlePacket(pkt);
    }
    // msg es un DistanceVector
    else if (DistanceVectorMsg *dv = dynamic_cast<DistanceVectorMsg*>(msg)) {
        handleDistanceVector(dv);

    }
    // msg es un helloPacket
    else if(Hello *hello = dynamic_cast<Hello*>(msg)){
        handleHello(hello);
    }
}


/* **** HANDLING FUNCTIONS **** */
void Net::handleHello(Hello *hello){
    int neighbour_gate = hello->getArrivalGate()->getIndex();
    int senderID = hello->getSenderID();

    RoutingEntry e;
    e.cost = 1;
    e.nextNode = senderID;

    routingTable[senderID] = e;
    gateTable[senderID] = neighbour_gate;

    sendTable();
    delete hello;
}


void Net::handleDistanceVector(DistanceVectorMsg *dv){
    bool DTableHasChanded = updateTable(dv);
    if (DTableHasChanded){
        sendTable();
    }
    delete dv;
}

void Net::handlePacket(Packet *pkt){
    int destination = pkt->getDestination();
    if (destination == this->getParentModule()->getIndex()) {
        send(pkt, "toApp$o");
    }
    else{
        auto it = routingTable.find(destination);
        if (it != routingTable.end()){
            int nextNode = it->second.nextNode;
            int gate = gateTable[nextNode];
            send(pkt, "toLnk$o", gate);
        }
        else{
            delete pkt;
        }
    }
}

/* **** DISTANCE TABLE FUNCS **** */

// func para enviar la distance table a nodos vecinos
void Net::sendTable(){
    DistanceVectorMsg *DVmsg = new DistanceVectorMsg("dv");
    DVmsg->setSenderId(this->id);
    DVmsg->setDistanceVectorArraySize(routingTable.size());

    int i = 0;
    for (auto it = routingTable.begin(); it != routingTable.end(); ++it){
        DistanceEntry distance_entry;

        distance_entry.destination = it->first;
        distance_entry.cost = it->second.cost;
        DVmsg->setDistanceVector(i, distance_entry);
        i++;
    }

    for (auto it = gateTable.begin(); it != gateTable.end(); ++it){
        if (it->second != -1){
            send(DVmsg->dup(), "toLnk$o", it->second);
        }
    }
    delete DVmsg;
}

// func para actualizar la distance table cuando me llega una
bool Net::updateTable(DistanceVectorMsg * dv){
    bool updated = false;
    int gateIn = dv->getArrivalGate()->getIndex();

    for (int i = 0; i < dv->getDistanceVectorArraySize(); i++){
        DistanceEntry de = dv->getDistanceVector(i);

        auto it = routingTable.find(de.destination);
        int total_cost = de.cost + 1;



        if (de.destination == this->id) {
            continue;
        }
        if (total_cost < 0 || (total_cost == 0 && de.destination != this->id)) {
            continue;
        }

        if (it == routingTable.end()){
            // es una nueva destination para nuestra routing table
            RoutingEntry newRoutingEntry;
            newRoutingEntry.cost = total_cost;
            newRoutingEntry.nextNode = dv->getSenderId();
            routingTable[de.destination] = newRoutingEntry;
            gateTable[dv->getSenderId()] = gateIn;
            updated = true;
        }
        else{
            // destination ya estaba en nuestra routing table
            RoutingEntry& existingEntry = it->second;
            if (total_cost < existingEntry.cost){
                existingEntry.cost = total_cost;
                existingEntry.nextNode = dv->getSenderId();
                gateTable[dv->getSenderId()] = gateIn;
                updated = true;
            }
            else if( existingEntry.nextNode == dv->getSenderId() && total_cost > existingEntry.cost){
                existingEntry.cost = total_cost;
                updated = true;
            }
        }
    }

    return updated;
}
