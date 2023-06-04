/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include <mutex>
#include <string>
#include <queue>

#include "LwsAdapter/ILwsSession.hpp"

namespace ews::cli
{

class LwsSession : public ILwsSession
{
public:
    LwsSession(LwsInstanceRawPtr);

    auto getLwsInstance() -> LwsInstanceRawPtr override;
    void addMessage(const std::string&) override;
    auto getMessages() -> std::queue<std::string>& override;

private:
    LwsInstanceRawPtr _wsInstance;
    std::queue<std::string> _messages;
    std::queue<std::string> _messagesToSend;
    std::mutex _mutex;
};

} // namespace ews::cli
