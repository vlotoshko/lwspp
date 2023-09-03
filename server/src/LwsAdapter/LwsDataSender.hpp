/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "easywebsocket/server/IDataSender.hpp"

#include "LwsAdapter/LwsTypesFwd.hpp"

namespace ews::srv
{

class LwsDataSender : public IDataSender
{
public:
    LwsDataSender(ILwsSessionsPtr s, ILwsCallbackNotifierPtr n);

    void sendTextData(SessionId sessionId, const std::string&) override;
    void sendBinaryData(SessionId sessionId, const std::vector<char>&) override;

    void sendTextData(const std::string&) override;
    void sendBinaryData(const std::vector<char>&) override;

private:
    ILwsSessionsPtr _sessions;
    ILwsCallbackNotifierPtr _notifier;
};

} // namespace ews::srv
