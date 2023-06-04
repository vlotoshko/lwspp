/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include <stdexcept>

#include "easywebsocket/client/IEventHandler.hpp"

#include "LwsAdapter/LwsCallbackContext.hpp"
#include "LwsAdapter/LwsMessageSender.hpp"

namespace ews::cli
{

LwsCallbackContext::LwsCallbackContext(IEventHandlerPtr e)
    : _eventHandler(std::move(e))
{}

void LwsCallbackContext::setStopping()
{
    _isStopping = true;
}

bool LwsCallbackContext::isStopping() const
{
    return _isStopping;
}

auto LwsCallbackContext::getEventHandler() -> IEventHandlerPtr
{
    return _eventHandler;
}

ILwsSessionPtr LwsCallbackContext::getSession()
{
    return _session;
}

void LwsCallbackContext::setSession(ILwsSessionPtr s)
{
    if (s == nullptr)
    {
        throw std::runtime_error{"unaintialized lws session found"};
    }
    _session = std::move(s);
    _eventHandler->setMessageSender(std::make_shared<LwsMessageSender>(_session));
}

void LwsCallbackContext::resetSession()
{
    _session.reset();
}

} // namespace ews::cli
