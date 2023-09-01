/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include <mutex>
#include <queue>

#include "LwsAdapter/ILwsSession.hpp"

namespace ews::cli
{

class LwsSession : public ILwsSession
{
public:
    LwsSession(LwsInstanceRawPtr);

    auto getLwsInstance() -> LwsInstanceRawPtr override;

    void addBinaryData(const std::vector<char>&) override;
    void addTextData(const std::string&) override;
    auto getMessages() -> std::queue<Message>& override;

private:
    LwsInstanceRawPtr _wsInstance;
    std::queue<Message> _messages;
    std::queue<Message> _messagesToSend;
    std::mutex _mutex;
};

} // namespace ews::cli
