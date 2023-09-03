/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jun, 2023
 */

#pragma once

#include "easywebsocket/server/Types.hpp"

namespace ews::srv
{

/**
 * @brief The ISessionInfo class provides the information about the connected session
 */
class ISessionInfo
{
public:
    ISessionInfo() = default;
    virtual ~ISessionInfo() = default;

    ISessionInfo(ISessionInfo&&) = default;
    auto operator=(ISessionInfo&&) noexcept -> ISessionInfo& = default;

    ISessionInfo(const ISessionInfo&) = delete;
    auto operator=(const ISessionInfo&) noexcept -> ISessionInfo& = delete;

public:
    virtual auto getSessionId() -> SessionId = 0;
    virtual auto getPath() -> const Path& = 0;
};

} // namespace ews::srv
