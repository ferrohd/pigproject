#include <functional>
#include "server.h"

using namespace Pistache;

void Server::onRequest(const Http::Request& request, Http::ResponseWriter response) {
     Http::serveFile(response, "src/index.html");
}

void Server::startServing(){
    std::cout << "Starting server..." << std::endl;
    Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(port));

    auto opts = Http::Endpoint::options().threads(1);
    Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(std::make_shared<Server>());
    std::cout << "Server up and running on port " << port << std::endl;
    server.serve();
}

void Server::run() {
    t = std::thread(std::bind(&Server::startServing, this));
}

