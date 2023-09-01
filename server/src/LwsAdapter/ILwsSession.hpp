/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <queue>
#include <string>

#include "easywebsocket/server/Types.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"

namespace ews::srv
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

    virtual void addBinaryData(const std::vector<char>&) = 0;
    virtual void addTextData(const std::string&) = 0;
    virtual auto getMessages() -> std::queue<Message>& = 0;
};

} // namespace ews::srv
