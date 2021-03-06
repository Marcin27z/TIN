//
// Created by Marcin on 14.05.2019.
//

#include "ClientInOutAction.h"

void ClientInOutAction::send() {

    if (!writing) {
        auto response = outQueue.get();
        message = serializer.serialize(std::move(response));
        char s[message.size() + 4];
        sprintf(s, "%04lu%s", message.size(), message.c_str());
        message = s;
        bytesToWrite = sizeof(s);
        bytesWritten = 0;
        writing = !writing;
    }

    auto status = write(fd, message.c_str() + bytesWritten, bytesToWrite);
    if (status != -1) {
        bytesWritten += status;
    }

    if (bytesWritten == bytesToWrite) {
        writing = !writing;
//        wantsToWrite =  false;

    }
}

void ClientInOutAction::receive() {

    if (!readingHeader && !payloadAllocated) {
        payloadAllocated = true;
        payload = new char[bytesToRead];
    }

    auto status = read(fd, readingHeader ? header + bytesRead : payload + bytesRead, bytesToRead - bytesRead);
    if (status > 0) {
        bytesRead += status;
    } else if (status == 0) {
        std::cout << "Disconnected user ;<" << std::endl;
        connected = false;
    } else if (status == -1) {
        std::cout << "Error!" << std::endl;
    }

    if (readingHeader && bytesToRead == bytesRead) {
        readingHeader = false;
        bytesRead = 0;
        bytesToRead = strtol(header, NULL, 10);
    }

    if (!readingHeader && bytesToRead == bytesRead) {
        readingHeader = true;
        auto request = deserializer.getDeserializedObject(payload);
        if (request != nullptr) {
            inQueue.put(std::move(request));
        }
//        delete[] payload;
        bytesToRead = 4;
        bytesRead = 0;
        payloadAllocated = false;
    }

}


ClientInOutAction::ClientInOutAction(int fd, SynchronizedQueue<std::unique_ptr<Request>> &inQueue,
                                     SynchronizedQueue<std::unique_ptr<Serializable>> &outQueue, bool &wantsToWrite, bool &connected)
        : inQueue(inQueue), outQueue(outQueue), wantsToWrite(wantsToWrite), connected(connected) {
    this->fd = fd;
}

