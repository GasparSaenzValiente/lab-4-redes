#ifndef NET
#define NET

#include <string.h>
#include <omnetpp.h>
#include <packet_m.h>
#include <distanceVector_m.h>
#include <hello_m.h>
#include <map>#include <limits>

using namespace omnetpp;


const int INFINITY_COST = std::numeric_limits<int>::max();

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
    virtual bool updateTable(DistanceVector * dv, int gate);
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

    routingTable.insert({id, {0, -1}});
    Hello * helloPkt = new Hello("helloMsg");
    helloPkt->setSenderID(id);
    for(int i = 0; i < interfaces; i++){
        send(helloPkt->dup(), "toLnk$o", i);
    }
    delete helloPkt;
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
        send(dv->dup(), "toLnk$o", i);
    }
    delete dv;
}

// func para actualizar la distance table cuando me llega una
bool Net::updateTable(DistanceVector * dv, int gate){
    int dv_size = dv->getEntriesArraySize();
    int senderID = dv->getSenderId();

    bool update = false;

    for (int i = 0; i < dv_size; i++){
        DistanceEntry distanceE = dv->getEntries(i);

        auto it = routingTable.find(distanceE.destination);
        if (it == routingTable.end()){ // el nodo destinatario es nuevo{
            RoutingEntry newEntry;
            newEntry.cost = distanceE.cost + 1; // +1 por el salto de ir al noda que envio el dv
            newEntry.nextNode = gate;
            routingTable[distanceE.destination] = newEntry;
            update = true;
        }
        else{ // el nodo destinatario esta en nuestra dtable, y hay que ver el minimo
            RoutingEntry existingEntry = it->second;

            // si el costo es menor hago cosas
            if (existingEntry.cost > distanceE.cost + 1 || existingEntry.cost == -1){
                existingEntry.cost = distanceE.cost + 1;
                existingEntry.nextNode = gate;
                routingTable[distanceE.destination] = existingEntry;
                update = true;
            }
            // si no lo dejo como esta, skip if
        }
    }
    return update;
}




void Net::handleMessage(cMessage *msg) {

    // msg es un PACKET
    if (Packet *pkt = dynamic_cast<Packet*>(msg)){
        EV << "LLEGO Pkt DE LA APP" << endl;
         // If this node is the final destination, send to App
        if (pkt->getDestination() == id) {
            EV << "PKT LLEGO A DESTINO " << endl;
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
            int destination = pkt->getDestination();
            EV << "DEBEMOS REENVIAR PKT " << endl;
            auto it = routingTable.find(destination);
            if (it != routingTable.end() && it->second.nextNode != -1) {
                EV << "DESTINATION SE ENCONTRABA EN LA ROUTING TABLE " << endl;
                int sendTo = it->second.nextNode;
                msg = (cMessage *) pkt;
                send(msg, "toLnk$o", sendTo);
            }
            else{
                // si no tenemos una entrada para ese que hago?
                // lo mano en sentido horario? lo borro?
                EV << "PACKET NO ESTABA EN RT" << endl;
                send(msg, "toLnk$o", 0);
                //delete pkt;
            }
        }
    }

    // msg es un DistanceVector
    else if (DistanceVector *dv = dynamic_cast<DistanceVector*>(msg)) {
        int gate = msg->getArrivalGate()->getIndex();
        bool update = updateTable(dv, gate);
        if (update){
            sendTable();
        }
        delete dv;

    }
    else if(Hello *helloPkt = dynamic_cast<Hello*>(msg)){
        int neighbour_gate = msg->getArrivalGate()->getIndex();
        int senderID = helloPkt->getSenderID();

        routingTable[senderID] = {1, neighbour_gate};
        sendTable();
        delete helloPkt;
    }
}
