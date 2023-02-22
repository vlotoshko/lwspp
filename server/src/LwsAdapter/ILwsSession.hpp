/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <queue>
#include <string>

#include "websocketpp/server/Types.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"

namespace wspp::srv
{

class ILwsSession
{
public:
    ILwsSession() = default;
    virtual ~ILwsSession() = default;

    ILwsSession(const ILwsSession&) = default;
    auto operator=(const ILwsSession&) -> ILwsSession& = default;

    ILwsSession(ILwsSession&&) noexcept = default;
    auto operator=(ILwsSession&&) noexcept -> ILwsSession& = default;

public:
    virtual auto getSessionId() const -> SessionId = 0;
    virtual auto getLwsInstance() -> LwsInstanceRawPtr = 0;

    virtual void addMessage(const std::string&) = 0;
    virtual auto getMessages() -> std::queue<std::string>& = 0;
};

} // namespace wspp::srv
