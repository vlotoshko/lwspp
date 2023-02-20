/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <queue>
#include <string>

namespace wspp::srv
{

class ISession
{
public:
    ISession() = default;
    virtual ~ISession() = default;

    ISession(const ISession&) = default;
    auto operator=(const ISession&) -> ISession& = default;

    ISession(ISession&&) noexcept = default;
    auto operator=(ISession&&) noexcept -> ISession& = default;

public:
    virtual void addMessage(const std::string&) = 0;
    virtual auto getMessages() -> std::queue<std::string> = 0;
};

} // namespace wspp::srv
