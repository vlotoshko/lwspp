/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <vector>

#include "easywebsocket/server/Types.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"

namespace ews::srv
{

/**
 * @brief The ILwsSessions class represents a container for storing and accessing all ILwsSession instances.
 */
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

} // namespace ews::srv
