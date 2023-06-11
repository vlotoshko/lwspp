/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "LwsAdapter/LwsServer.hpp"
#include "Server.hpp"

namespace ews::srv
{

Server::Server(const ServerContext& context)
    : _lwsServer(std::make_shared<LwsServer>(context))
{
    std::weak_ptr<LwsServer> weakLwsServer = _lwsServer;

    auto asyncStartListening = [weakLwsServer]
    {
        if (auto lwsServer = weakLwsServer.lock())
        {
            lwsServer->startListening();
        }
    };

    _serverStop = std::async(std::launch::async, asyncStartListening);
}

Server::~Server()
{
    _lwsServer->stopListening();
    _serverStop.wait();
}

} // namespace ews::srv
