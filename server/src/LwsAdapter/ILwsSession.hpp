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

/**
 * @brief The ILwsSession class represents the connection to the client.
 */
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

    virtual void addBinaryDataToSend(const std::vector<char>&) = 0;
    virtual void addTextDataToSend(const std::string&) = 0;
    virtual auto getPendingData() -> std::queue<Message>& = 0;
};

} // namespace ews::srv
