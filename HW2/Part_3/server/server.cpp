#include <zmq.hpp>
#include <string>
#include <unistd.h>
#include <assert.h>
#include <iostream>

int main (void)
{
    zmq::context_t context(1);
    zmq::socket_t responder(context, ZMQ_REP);
    responder.bind("tcp://*:5567");

    //Track the current iteration for each client
    int clientsIteration[9];
    //Track the number of connected clients
    int numClientIDs = 0;
    //Track the currently connected client
    int currentClientID = 0;

    bool running = true;
    while (running) {
        //Create buffer for incoming message
        zmq::message_t buffer;
        //Receive the message through the buffer
        responder.recv(buffer);
        int bufferSize = buffer.size();
        std::string bufferString = std::string(static_cast<char *>(buffer.data()), buffer.size());
        
        //Print out the incoming message - Can uncomment to print and view what the server is currently working on.
        // printf ("Received Message: ");
        // std::cout << bufferString << "\n";

        //Check if server received exit command
        if (bufferSize > 4 && bufferString.substr(0, 4) == "Exit") {
            running = false;
            std::string str = bufferString.substr(4);
            currentClientID = std::stoi(str);

        //New client connected
        //Increment the number of clients connected and use that as the ID for the client
        //Set the current iteration to 0
        } else if (bufferString == "-1") {
            numClientIDs++;
            currentClientID = numClientIDs;
            clientsIteration[currentClientID] = 0;

        //Previously connected client connected again
        } else {
            std::string str = bufferString;
            currentClientID = std::stoi(str);
        }
        //Increment current connected clients iteration
        clientsIteration[currentClientID]++;

        //Construct message to send
        //Send client ID as first character then a comma, followed by all clients and their iterations
        std::string msgToSend = "";
        msgToSend.assign(std::to_string(currentClientID) + ",");
        for (int i = 1; i <= numClientIDs; i++) {
            msgToSend.append("Client " + std::to_string(i) + ": Iteration " + std::to_string(clientsIteration[i]) + "\n");
        }

        //Send message to client
        zmq::message_t msg(msgToSend.length());
        memcpy(msg.data(), msgToSend.c_str(), msgToSend.length());
        responder.send(msg);        
    }

    //Close responder socket
    std::cout << "Shutting Down Server\n";
    responder.close();
    return 0;
}