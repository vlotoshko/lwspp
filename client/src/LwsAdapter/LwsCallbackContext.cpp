/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#include <stdexcept>

#include "easywebsocket/client/IMessageSenderAcceptor.hpp"

#include "LwsAdapter/LwsCallbackContext.hpp"
#include "LwsAdapter/LwsMessageSender.hpp"

namespace ews::cli
{

LwsCallbackContext::LwsCallbackContext(IEventHandlerPtr e, IMessageSenderAcceptorPtr a)
    : _eventHandler(std::move(e))
    ,_messageSenderAcceptor(std::move(a))
{}

void LwsCallbackContext::setStopping()
{
    _isStopping = true;
}

auto LwsCallbackContext::isStopping() const -> bool
{
    return _isStopping;
}

auto LwsCallbackContext::getEventHandler() -> IEventHandlerPtr
{
    return _eventHandler;
}

auto LwsCallbackContext::getSession() -> ILwsSessionPtr
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

    auto messageSender = std::make_shared<LwsMessageSender>(_session);
    _messageSenderAcceptor->acceptMessageSender(std::move(messageSender));
}

void LwsCallbackContext::resetSession()
{
    _session.reset();
}

} // namespace ews::cli
