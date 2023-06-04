/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "easywebsocket/server/IMessageSender.hpp"

#include "LwsAdapter/LwsTypesFwd.hpp"

namespace ews::srv
{

class LwsMessageSender : public IMessageSender
{
public:
    LwsMessageSender(ILwsSessionsPtr s, ILwsCallbackNotifierPtr n);

    void sendMessage(SessionId sessionId, const std::string& message) override;
    void sendMessage(const std::string& message) override;

private:
    ILwsSessionsPtr _sessions;
    ILwsCallbackNotifierPtr _notifier;
};

} // namespace ews::srv
