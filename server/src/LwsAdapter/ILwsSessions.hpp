/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <vector>

#include "websocketpp/server/Types.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"

namespace wspp::srv
{

class ILwsSessions
{
public:
    ILwsSessions() = default;
    virtual ~ILwsSessions() = default;

    ILwsSessions(const ILwsSessions&) = default;
    auto operator=(const ILwsSessions&) -> ILwsSessions& = default;

    ILwsSessions(ILwsSessions&&) noexcept = default;
    auto operator=(ILwsSessions&&) noexcept -> ILwsSessions& = default;

public:
    virtual void add(ILwsSessionPtr) = 0;
    virtual void remove(SessionId) = 0;
    virtual auto get(SessionId) -> ILwsSessionPtr = 0;
    virtual auto getAllSessions() -> std::vector<ILwsSessionPtr> = 0;
};

} // namespace wspp::srv
