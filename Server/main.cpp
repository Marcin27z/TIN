#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <cstring>
//#include <zconf.h>
#include <pthread.h>
#include <vector>
#include <poll.h>
#include <climits>
#include <map>
#include <bsoncxx/builder/stream/document.hpp>
#include "ServerSocket.h"
#include "containers/synchronizedcontainers/SynchronizedVector.h"
#include "Dao.h"
#include "entity/Account.h"
#include "entity/Consultation.h"
#include "containers/OutgoingMessage.h"
#include "containers/synchronizedcontainers/SynchronizedQueue.h"
#include "serialization/Serializer.h"
#include "dto/DailyConsultationsListResponse.h"
#include "serialization/Deserializer.h"
#include "dto/enums/StatusType.h"
#include "dto/LoginResponse.h"
#include "threads/TCPThread.h"
#include "threads/MessageSender.h"
#include <jsoncpp/json/json.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
using namespace std;

enum Commands {
    stopCommand,
    restartCommand,
    changePortCommand,
    sendToCommand
};
map<string, Commands> mapStringCommands;

void initialize() {
    mapStringCommands["stop"] = stopCommand;
    mapStringCommands["restart"] = restartCommand;
    mapStringCommands["changeport"] = changePortCommand;
    mapStringCommands["sendto"] = sendToCommand;
}

in_port_t getPortFromArgument(char *arg) {
    in_port_t port;
    char *endPtr = nullptr;
    errno = 0;
    auto argumentPort = strtoul(arg, &endPtr, 10);
    if (arg == endPtr) {
        throw invalid_argument("port invalid: no digits found");
    } else if (errno == ERANGE && argumentPort == LONG_MIN) {
        throw invalid_argument("port invalid: underflow occurred");
    } else if (errno == ERANGE && argumentPort == LONG_MAX) {
        throw invalid_argument("port invalid: overflow occurred");
    } else if (errno == EINVAL) {
        throw invalid_argument("port invalid: base contains unsupported value");
    } else if (errno != 0 && argumentPort == 0) {
        throw invalid_argument("port invalid: unspecified error occurred");
    } else if (errno == 0 && arg) {
        if (argumentPort < USHRT_MAX) {
            port = static_cast<in_port_t>(argumentPort);
        } else {
            throw invalid_argument("port invalid: port number too big");
        }
    } else {
        throw invalid_argument("port invalid: dunno");
    }
    return port;
}

int main(int argc, char *argv[]) {
    in_port_t port = 9999; // default port
    if (argc > 1) {
        try {
            port = getPortFromArgument(argv[1]);
            cout << "starting on port: " << port << endl;
        } catch (invalid_argument &e) {
            cout << e.what() << endl << "starting on default port(" << port << ")" << endl;
        }
    }
    auto running = true;
    TCPThread tcpThread(port);
    tcpThread.start();

    initialize();



    /*
    std::cout<<"Proba logowania"<<std::endl;
    auto dao = Dao::getDaoCollection("TIN", "consultation");
    auto result = dao->getConsultationById("5ced676418cdd14b0579fb19");
    std::cout << result.getConsultationDateStart();
    */


    char value[256];
    while (running) {
        string message;
        cin >> value;
        switch (mapStringCommands[value]) {
            case stopCommand:
//                cout << "Stopping server..." << endl;
//                tcpThread.cancel();
//                running = false;
                break;
            case changePortCommand:
//                tcpThread.changePort(9998);
                break;
            case restartCommand:
                break;
            case sendToCommand:
                break;
            default:
                break;
        }
    }
    tcpThread.join();
    cout << "Server stopped." << endl;
    return 0;
}


#pragma clang diagnostic pop