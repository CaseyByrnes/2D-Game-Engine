#include <zmq.hpp>
#include <string>
#include <iostream>
#include <unistd.h>

int main (void)
{   
    const int iterations = 5;
    printf("Connecting to Server\n");
    zmq::context_t context(1);
    zmq::socket_t requester(context, ZMQ_REQ);
    requester.connect("tcp://localhost:5567");

    //Initial message for request
    //Default is -1 for new client, the server will send an ID after first request sent
    std::string msgID = "-1";

    int request_nbr;
    for (request_nbr = 0; request_nbr < iterations; request_nbr++) {
        //If last request number, send exit with request to server
        if (request_nbr == iterations - 1) {
            std::string exitString = "Exit" + msgID;
            msgID.assign(exitString);
        }
        //Send request
        zmq::message_t msgSnd(msgID.length());
        memcpy(msgSnd.data(), msgID.c_str(), msgID.length());
        requester.send(msgSnd);

        //Receive response from server
        zmq::message_t buffer;
        requester.recv(buffer);
        int bufferSize = buffer.size();
        std::string bufferString = std::string(static_cast<char *>(buffer.data()), buffer.size());

        //Print out response, minus the ID
        std::cout << bufferString.substr(2, bufferSize);
        
        //If this is the first reply from the server then change the ID
        if (msgID == "-1") {
            //Set clients request message to ID that the server assigned.
            msgID.assign(bufferString.substr(0,1));
        }
        sleep(1);
    }

    //Close requester socket
    requester.close();
    return 0;
}