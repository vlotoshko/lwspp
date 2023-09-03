/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include "LwsAdapter/ILwsCallbackNotifier.hpp"
#include "LwsAdapter/ILwsSession.hpp"
#include "LwsAdapter/ILwsSessions.hpp"
#include "LwsAdapter/LwsDataSender.hpp"

namespace ews::srv
{

LwsDataSender::LwsDataSender(ILwsSessionsPtr s, ILwsCallbackNotifierPtr n)
    : _sessions(std::move(s))
    , _notifier(std::move(n))
{}

void LwsDataSender::sendTextData(SessionId sessionId, const std::string& message)
{
    if (auto session = _sessions->get(sessionId))
    {
        session->addTextDataToSend(message);
        _notifier->notifyPendingDataAdded(session);
    }
}

void LwsDataSender::sendBinaryData(SessionId sessionId, const std::vector<char>& data)
{
    if (auto session = _sessions->get(sessionId))
    {
        session->addBinaryDataToSend(data);
        _notifier->notifyPendingDataAdded(session);
    }
}

void LwsDataSender::sendTextData(const std::string& message)
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

void LwsDataSender::sendBinaryData(const std::vector<char>& data)
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
