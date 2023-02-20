/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include "websocketpp/server/IMessageSender.hpp"
#include "EventHandler.hpp"

namespace wspp::chat
{

void EventHandler::onConnect(wspp::srv::SessionId sessionId) noexcept
{
    if (_messageSender != nullptr)
    {
        _messageSender->sendMessage(std::string{"welcome new user, his number is: "}
                                    .append(std::to_string(sessionId)));
    }
}

void EventHandler::onDisconnect(wspp::srv::SessionId) noexcept
{

}

void EventHandler::onError(srv::SessionId, const std::string& /*errorMessage*/) noexcept
{

}

void EventHandler::setMessageSender(srv::IMessageSenderPtr messageSender)
{
    _messageSender = std::move(messageSender);
}

} // namespace wspp::chat
