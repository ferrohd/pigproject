#ifndef SERVER_H
#define SERVER_H

#include <iostream>

#include "pistache/endpoint.h"

class Server : public Pistache::Http::Handler {

private:
	std::thread t;
	int port;

public:
	Server(): port(9000) {
		
	}
	~Server() {
        std::cout << "exiting server..." << std::endl;
    }

    HTTP_PROTOTYPE(Server)

    void onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response);
    void startServing();
    void run();
};

#endif
