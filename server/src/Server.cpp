/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include <map>
#include <mutex>

#include "LwsAdapter/LwsContext.hpp"
#include "Server.hpp"
#include "ServerContext.hpp"

namespace ews::srv
{
namespace
{

std::map<int, std::function<void()>> stoppers;
std::mutex registeredStoppersMutex;

void signalHandler(int signum)
{
    const std::lock_guard<std::mutex> lock{registeredStoppersMutex};
    for (const auto& stopper : stoppers)
    {
        stopper.second();
    }
    exit(signum);
}

void registerSingnalHandling(int port, std::function<void()>&& stopper)
{
    const std::lock_guard<std::mutex> lock{registeredStoppersMutex};
    signal(SIGINT, signalHandler);

    auto it = stoppers.find(port);
    if (it == stoppers.end())
    {
        stoppers[port] = std::move(stopper);
    }
    else
    {
        throw std::runtime_error{"Server with such port already exists: " + std::to_string(port)};
    }
}

void unregisterSingnalHandling(int port)
{
    const std::lock_guard<std::mutex> lock{registeredStoppersMutex};
    stoppers.erase(port);
}

} // namespace

Server::Server(const ServerContext& context)
    : _lwsContext(context)
{
    registerSingnalHandling(_lwsContext.getPort(), [this] { stop_(); });
}

Server::~Server()
{
    stop_();
    unregisterSingnalHandling(_lwsContext.getPort());
}

void Server::start()
{
    _lwsContext.startListening();
}

void Server::stop_()
{
    _lwsContext.stopListening();
}

} // namespace ews::srv
