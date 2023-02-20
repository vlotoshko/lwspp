/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include "EventHandlerWrapper.hpp"
#include <iostream>

namespace wspp::srv
{

EventHandlerWrapper::EventHandlerWrapper(IEventHandlerPtr eventHandler,
                                         Server::Sessions& sessions,
                                         Server& server)
    : _eventHandler(std::move(eventHandler))
    , _sessions(sessions)
    , _server(server)
{}

void EventHandlerWrapper::onConnect(SessionId sessionId) noexcept
{
    _sessions[sessionId];
    _server.sendMessage(sessionId, "hello!");
    _eventHandler->onConnect(sessionId);
}

void EventHandlerWrapper::onDisconnect(SessionId sessionId) noexcept
{
    _sessions.erase(sessionId);
    _eventHandler->onDisconnect(sessionId);
}

} // namespace wspp::srv
