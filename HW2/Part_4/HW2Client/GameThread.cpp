#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <zmq.hpp>

#include "GameThread.h"
#include "character.h"
#include "floatingPlatform.h"
#include "InputSubsystem.h"

bool busy;
int id;
GameThread *other;
std::mutex *m;
std::condition_variable *cv;

GameThread::GameThread(int id, GameThread *other, std::mutex *m, std::condition_variable *cv)
{
    this->id = id;
    if(id == 0) { busy = true; };
    this->m = m;
    this->cv = cv;
}

bool GameThread::isBusy()
{
    std::lock_guard<std::mutex> lock(*m);
    return busy;
}

void GameThread::movePlatform(FloatingPlatform *platform, int movementSpeed)
{
    platform->move(m, cv, movementSpeed);
}

void GameThread::checkInputs(InputSubsystem *inputs, int movementSpeed)
{
    inputs->checkForInput(m, cv, movementSpeed);
}

void GameThread::connectToServer(bool *running, ClientStruct *clientInfo) {
    std::unique_lock<std::mutex> cv_lock(*m);

    //Create requester socket and connect to server
    zmq::context_t context(1);
    zmq::socket_t requester(context, ZMQ_REQ);
    requester.connect("tcp://localhost:5561");

    while (requester.connected()) {
        //If game loop is not longer running
        if (!*running) {
            //Set ID to negative to so server can handle the disconnect
            clientInfo->id = clientInfo->id * -1;
        }
        //Send ClientStruct as message data
        zmq::message_t buffer;
        memcpy(buffer.data(), &clientInfo, sizeof(ClientStruct));
        requester.send(buffer);

        //Receive a ClientStruct back from server
        zmq::message_t buffer2;
        requester.recv(buffer2);
        ClientStruct *receivedClient = static_cast<struct ClientStruct *>(buffer.data());

        //Update clients ClientStruct with new info from server
        clientInfo->characterPosition = receivedClient->characterPosition;
        clientInfo->id = receivedClient->id;
        clientInfo->paused = receivedClient->paused;

        //If game loop is not longer running
        if (!*running) {
            //Disconnect socket from server
            requester.disconnect("tcp://localhost:5561");
        }
    }
    //Close the socket
    requester.close();
}

void GameThread::connectClients(bool *running, ServerStruct *serverInfo, int *connectedClients) {
    std::unique_lock<std::mutex> cv_lock(*m);

    //Create responder socket and bind to port
    zmq::context_t context(1);
    zmq::socket_t responder(context, ZMQ_REP);
    responder.bind("tcp://*:5561");
    
    while (*running) {
        if (*connectedClients >= 0) {
            //Receive ClientStruct from client
            zmq::message_t buffer;
            responder.recv(buffer);
            ClientStruct *currentPlayer = static_cast<struct ClientStruct *>(buffer.data()); 
            // memcpy(&currentPlayer, buffer.data(), sizeof(ClientStruct));

            //Check if the connected client is new (ID = 0)
            if (currentPlayer->id == 0) {
                //Give the client a new ID
                currentPlayer->id = serverInfo->ids.size() + 1;
                //Add the new ID and the character position of the client character to the ServerStruct
                serverInfo->characterPositions.push_back(currentPlayer->characterPosition);
                serverInfo->ids.push_back(currentPlayer->id);
                //Increment the number of connected clients
                *connectedClients++;

            //Check if the connected client is wanting to disconnect (ID < 0)
            } else if (currentPlayer->id < 0) {
                //Conver the negative ID to positive
                currentPlayer->id = -currentPlayer->id;
                //Find existing client in the ServerStruct
                for (int i = 0; i < *connectedClients; i++) {
                    if (serverInfo->ids.at(i) == currentPlayer->id) {
                        //Remove the client ID and character position from the ServerStruct
                        serverInfo->ids.erase(serverInfo->ids.begin() + i);
                        serverInfo->characterPositions.erase(serverInfo->characterPositions.begin() + i);
                        //Decrement the nuimber of connected clients
                        *connectedClients--;
                        break;
                    }
                }
            //The client is already connected and wants to stay connected
            } else {
                //Find existing client in the ServerStruct
                for (int i = 0; i < *connectedClients; i++) {
                    if (serverInfo->ids.at(i) == currentPlayer->id) {
                        //Update the position of the client's character in the ServerStruct
                        serverInfo->characterPositions.at(i) = currentPlayer->characterPosition;
                        break;
                    }
                }
            }

            //Send the ClientStruct as the data of a message
            zmq::message_t sendBuffer;
            memcpy(sendBuffer.data(), &currentPlayer, sizeof(ClientStruct));
            responder.send(sendBuffer);

        }
        
    }
    //Close the responder socket
    responder.close();
}
