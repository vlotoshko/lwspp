/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <vector>

#include "websocketpp/server/Types.hpp"
#include "TypesFwd.hpp"

namespace wspp::srv
{

class ISessions
{
public:
    ISessions() = default;
    virtual ~ISessions() = default;

    ISessions(const ISessions&) = default;
    auto operator=(const ISessions&) -> ISessions& = default;

    ISessions(ISessions&&) noexcept = default;
    auto operator=(ISessions&&) noexcept -> ISessions& = default;

public:
    virtual void add(SessionId) = 0;
    virtual void remove(SessionId) = 0;
    virtual auto get(SessionId) -> ISessionPtr = 0;
    virtual auto getAllSessions() -> std::vector<ISessionPtr> = 0;
};

} // namespace wspp::srv
