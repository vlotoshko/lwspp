/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include "CallbackContext.hpp"

namespace wspp::srv
{

CallbackContext::CallbackContext(IEventHandlerPtr e, ISessionsPtr s)
    : _eventHandler(std::move(e))
    , _sessions(std::move(s))
{}

void CallbackContext::setServerStopped()
{
    _isStopped = true;
}

auto CallbackContext::isServerStopped() const -> bool
{
    return _isStopped;
}

auto CallbackContext::getEventHandler() -> IEventHandlerPtr
{
    return _eventHandler;
}

auto CallbackContext::getSessions() -> ISessionsPtr
{
    return _sessions;
}

} // namespace wspp::srv
