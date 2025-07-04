#ifndef LNK
#define LNK

#include "packet_m.h"
#include <omnetpp.h>
#include <string.h>

using namespace omnetpp;

class Lnk : public cSimpleModule {
private:
    cQueue buffer;
    cMessage* endServiceEvent = NULL;
    cOutVector bufferSizeVector;

public:
    Lnk();
    ~Lnk() override;

protected:
    void initialize() override;
    void finish() override;
    void handleMessage(cMessage* msg) override;
    void handleEndServiceEvent();
};

Define_Module(Lnk);

#endif /* LNK */

Lnk::Lnk() {
}

Lnk::~Lnk() {
    cancelAndDelete(endServiceEvent);
}

void Lnk::initialize() {
    endServiceEvent = new cMessage("endService");
    bufferSizeVector.setName("BufferSize");
}

void Lnk::finish() {
}

void Lnk::handleMessage(cMessage* msg) {
    if (msg == endServiceEvent) {
        handleEndServiceEvent();
    } else {
        if (msg->arrivedOn("toNet$i")) {
            // enqueue
            buffer.insert(msg);
            bufferSizeVector.record(buffer.getLength());
            // if the server is idle
            if (!endServiceEvent->isScheduled()) {
                // start the service now
                scheduleAt(simTime() + 0, endServiceEvent);
            }
        } else {
            // msg is from out, send to net
            send(msg, "toNet$o");
        }
    }
}

void Lnk::handleEndServiceEvent() {
    if (buffer.isEmpty()) {
        return;
    }

    // en el buffer hacen enqueue de cMessage, por eso el casteo
    cMessage* msg = dynamic_cast<cMessage*>(buffer.pop());
    bufferSizeVector.record(buffer.getLength());

    // networkStar tiene Node.toNod[] sin conectar, lo que da error al querer mandar un msg
    if (!gate("toOut$o")->getNextGate()->isConnected()) {
        delete msg;
        return;
    }

    send(msg, "toOut$o");

    simtime_t serviceTime;
    // si el msg era un Packet de la app le ponemos service time
    if (Packet* pkt = dynamic_cast<Packet*>(msg)) {
        serviceTime = pkt->getDuration();
    }
    // si era de DV o hello lo mandamos de una
    else {
        serviceTime = 0;
    }

    scheduleAt(simTime() + serviceTime, endServiceEvent);
}