/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <mutex>
#include <string>
#include <queue>

#include "LwsAdapter/ILwsSession.hpp"

namespace ews::srv
{

class LwsSession : public ILwsSession
{
public:
    LwsSession(SessionId, LwsInstanceRawPtr);

    auto getSessionId() const -> SessionId override;
    auto getLwsInstance() -> LwsInstanceRawPtr override;

    void addBinaryDataToSend(const std::vector<char>&) override;
    void addTextDataToSend(const std::string&) override;
    auto getPendingData() -> std::queue<Message>& override;

private:
    SessionId _sessionId;
    LwsInstanceRawPtr _wsInstance;
    std::queue<Message> _pendingData;
    std::queue<Message> _pendingDataToSend;
    std::mutex _mutex;
};

} // namespace ews::srv
