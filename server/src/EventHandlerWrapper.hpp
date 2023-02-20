/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "websocketpp/server/IEventHandler.hpp"
#include "websocketpp/server/TypesFwd.hpp"

#include "Server.hpp"

namespace wspp::srv
{

class EventHandlerWrapper : public IEventHandler
{
public:
    explicit EventHandlerWrapper(IEventHandlerPtr, Server::Sessions&, Server&);

    void onConnect(SessionId) noexcept override;
    void onDisconnect(SessionId) noexcept override;

private:
    IEventHandlerPtr _eventHandler;
    Server::Sessions& _sessions;
    Server& _server;
};

} // namespace wspp::srv
