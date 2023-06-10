/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "LwsAdapter/LwsContext.hpp"
#include "Server.hpp"

namespace ews::srv
{

Server::Server(const ServerContext& context)
    : _lwsContext(std::make_shared<LwsContext>(context))
{
    _serverStop = std::async(std::launch::async, [&]{ _lwsContext->startListening(); });
}

Server::~Server()
{
    _lwsContext.reset();
    _serverStop.wait();
}

} // namespace ews::srv
