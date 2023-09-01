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

    void addBinaryData(const std::vector<char>&) override;
    void addTextData(const std::string&) override;
    auto getMessages() -> std::queue<Message>& override;

private:
    SessionId _sessionId;
    LwsInstanceRawPtr _wsInstance;
    std::queue<Message> _messages;
    std::queue<Message> _messagesToSend;
    std::mutex _mutex;
};

} // namespace ews::srv
