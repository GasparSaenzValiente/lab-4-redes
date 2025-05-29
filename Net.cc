#ifndef NET
#define NET

#include <string.h>
#include <omnetpp.h>
#include <packet_m.h>

using namespace omnetpp;

class Net: public cSimpleModule {
private:
    cOutVector delay;           // para trazar delay por paquete
    cStdDev    delayStats;      // para estadistica

    cOutVector hopCount;        // para trazar saltos por paquete
    cStdDev    hopStats;        // para estadistica

    cOutVector pktRcvVector;    // para guardar paquetes recibidos
    cStdDev    pktRcvStats;     // para estadistica
public:
    Net();
    virtual ~Net();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Net);

#endif /* NET */

Net::Net() {
}

Net::~Net() {
}

void Net::initialize() {
    // nombramos vectores y estadisticas
    delay.setName("Demora de entrega (Net Delay)");
    delayStats.setName("DelayStats(Net)");

    hopCount.setName("Numero de saltos (HopsPerPacket)");
    hopStats.setName("HopStats");

    pktRcvVector.setName("Paquetes recibidos");
    pktRcvStats.setName("PacketsStats");
}

void Net::finish() {
    // Delay
    recordScalar("Delay promedio", delayStats.getMean());
    recordScalar("StdDevDelay",   delayStats.getStddev());
    recordScalar("TotalDelay(net)", delayStats.getCount());

    // Saltos (hops)
    recordScalar("AverageHops",   hopStats.getMean());
    recordScalar("StdDevHops",    hopStats.getStddev());
    recordScalar("TotalHops",     hopStats.getCount());

    // Paquetes recibidos
    recordScalar("PacketCount",   pktRcvStats.getCount());
    recordScalar("StdDevPacketsReceived", pktRcvStats.getStddev());
    recordScalar("TotalPacketsReceived(net)", pktRcvStats.getCount());
}

void Net::handleMessage(cMessage *msg) {

    // All msg (events) on net are packets
    Packet *pkt = (Packet *) msg;

    // If this node is the final destination, send to App
    if (pkt->getDestination() == this->getParentModule()->getIndex()) {
        // llegada a destino
        simtime_t delayTime = simTime() - pkt->getCreationTime();
        delay.record(delayTime);
        delayStats.collect(delayTime);
        // EV<< "Delay = " <<delayTime<<endl;

        // esto registra los saltos finales
        hopCount.record(pkt->getHopCount());
        hopStats.collect(pkt->getHopCount());

        // paquetes recibidos
        pktRcvVector.record(1);
        pktRcvStats.collect(1);

        send(msg, "toApp$o");
    }
    // If not, forward the packet to some else... to who?
    else {
        // Incrementamos el hopCount
        pkt->setHopCount(pkt->getHopCount() + 1);
        // actualizamos la informacion
        hopCount.record(pkt->getHopCount()); 
        hopStats.collect(pkt->getHopCount());

        msg = (cMessage *) pkt;
        send(msg, "toLnk$o", 0);
    }
}
