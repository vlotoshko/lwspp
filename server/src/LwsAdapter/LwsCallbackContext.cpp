/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include "LwsAdapter/LwsCallbackContext.hpp"

namespace ews::srv
{

LwsCallbackContext::LwsCallbackContext(IEventHandlerPtr e, ILwsSessionsPtr s)
    : _eventHandler(std::move(e))
    , _sessions(std::move(s))
{}

void LwsCallbackContext::setStopping()
{
    _isStopping = true;
}

auto LwsCallbackContext::isStopping() const -> bool
{
    return _isStopping;
}

auto LwsCallbackContext::getSessions() -> ILwsSessionsPtr
{
    return _sessions;
}

auto LwsCallbackContext::getEventHandler() -> IEventHandlerPtr
{
    return _eventHandler;
}


} // namespace ews::srv
