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
        session->addTextDataToSend(message);
        _notifier->notifyPendingDataAdded(session);
    }
}

void LwsMessageSender::sendData(SessionId sessionId, const std::vector<char>& data)
{
    if (auto session = _sessions->get(sessionId))
    {
        session->addBinaryDataToSend(data);
        _notifier->notifyPendingDataAdded(session);
    }
}

void LwsMessageSender::sendMessage(const std::string& message)
{
    for(auto& entry : _sessions->getAllSessions())
    {
        if (entry != nullptr)
        {
            entry->addTextDataToSend(message);
        }
        else
        {
            // TODO: log warning
        }
    }
    _notifier->notifyPendingDataAdded();
}

void LwsMessageSender::sendData(const std::vector<char>& data)
{
    for(auto& entry : _sessions->getAllSessions())
    {
        if (entry != nullptr)
        {
            entry->addBinaryDataToSend(data);
        }
        else
        {
            // TODO: log warning
        }
    }
    _notifier->notifyPendingDataAdded();
}

} // namespace ews::srv
