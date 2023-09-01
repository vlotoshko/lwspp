/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include "LwsAdapter/ILwsCallbackNotifier.hpp"
#include "LwsAdapter/ILwsSession.hpp"
#include "LwsAdapter/ILwsSessions.hpp"
#include "LwsAdapter/LwsMessageSender.hpp"

namespace ews::srv
{

LwsMessageSender::LwsMessageSender(ILwsSessionsPtr s, ILwsCallbackNotifierPtr n)
    : _sessions(std::move(s))
    , _notifier(std::move(n))
{}

void LwsMessageSender::sendMessage(SessionId sessionId, const std::string& message)
{
    if (auto session = _sessions->get(sessionId))
    {
        session->addTextData(message);
        _notifier->sendSessionMessages(session);
    }
}

void LwsMessageSender::sendData(SessionId sessionId, const std::vector<char>& data)
{
    if (auto session = _sessions->get(sessionId))
    {
        session->addBinaryData(data);
        _notifier->sendSessionMessages(session);
    }
}

void LwsMessageSender::sendMessage(const std::string& message)
{
    for(auto& entry : _sessions->getAllSessions())
    {
        if (entry != nullptr)
        {
            entry->addTextData(message);
        }
        else
        {
            // TODO: log warning
        }
    }
    _notifier->sendPendingMessages();
}

void LwsMessageSender::sendData(const std::vector<char>& data)
{
    for(auto& entry : _sessions->getAllSessions())
    {
        if (entry != nullptr)
        {
            entry->addBinaryData(data);
        }
        else
        {
            // TODO: log warning
        }
    }
    _notifier->sendPendingMessages();
}

} // namespace ews::srv
