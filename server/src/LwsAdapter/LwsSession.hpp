/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <mutex>
#include <string>
#include <queue>

#include "LwsAdapter/ILwsSession.hpp"

namespace wspp::srv
{

class LwsSession : public ILwsSession
{
public:
    LwsSession(SessionId, LwsInstanceRawPtr);

    auto getSessionId() const -> SessionId override;
    auto getLwsInstance() -> LwsInstanceRawPtr override;

    void addMessage(const std::string&) override;
    auto getMessages() -> std::queue<std::string>& override;

private:
    SessionId _sessionId;
    LwsInstanceRawPtr _wsInstance;
    std::queue<std::string> _messages;
    std::queue<std::string> _messagesToSend;
    std::mutex _mutex;
};

} // namespace wspp::srv
