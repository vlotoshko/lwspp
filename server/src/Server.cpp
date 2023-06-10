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
    _serverStop = std::async(std::launch::async, [&]{ _lwsServer->startListening(); });
}

Server::~Server()
{
    _lwsServer.reset();
    _serverStop.wait();
}

} // namespace ews::srv
