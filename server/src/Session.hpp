/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <mutex>
#include <string>
#include <queue>

#include "ISession.hpp"
#include "TypesFwd.hpp"

namespace wspp::srv
{

class Session : public ISession
{
public:
    void addMessage(const std::string&) override;
    auto getMessages() -> std::queue<std::string> override;
    lws* getLwsInstance();
private:
    lws* _wsInstance;
    std::queue<std::string> _messages;
    std::mutex _mutex;
};

} // namespace wspp::srv
