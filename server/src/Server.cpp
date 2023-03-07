/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include <map>
#include <mutex>

#include "LwsAdapter/LwsContext.hpp"
#include "Server.hpp"
#include "ServerContext.hpp"

namespace wspp::srv
{
namespace
{

std::map<int, Server*> registeredServers;
std::mutex registeredServersMutex;

void signalHandler(int signum)
{
    const std::lock_guard<std::mutex> lock{registeredServersMutex};
    for (auto server : registeredServers)
    {
        server.second->stop();
    }
    exit(signum);
}

void registerSingnalHandling(int port, Server* server)
{
    const std::lock_guard<std::mutex> lock{registeredServersMutex};
    signal(SIGINT, signalHandler);

    auto it = registeredServers.find(port);
    if (it == registeredServers.end())
    {
        registeredServers[port] = server;
    }
    else
    {
        throw std::runtime_error{"Server with such port already exists: " + std::to_string(port)};
    }
}

void unregisterSingnalHandling(int port)
{
    const std::lock_guard<std::mutex> lock{registeredServersMutex};
    registeredServers.erase(port);
}

} // namespace

Server::Server(const ServerContext& context)
    : _lwsContext(context)
{
    registerSingnalHandling(_lwsContext.getPort(), this);
}

void Server::start()
{
    _lwsContext.startListening();
}

void Server::stop()
{
    stop_();
}

void Server::stop_()
{
    _lwsContext.stopListening();
}

Server::~Server()
{
    stop_();
    unregisterSingnalHandling(_lwsContext.getPort());
}

} // namespace wspp::srv
