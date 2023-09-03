/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "easywebsocket/server/TypesFwd.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"

namespace ews::srv
{

/**
 * @brief The ILwsCallbackContext class provides data required by LwsCallback functions
 */
class ILwsCallbackContext
{
public:
    ILwsCallbackContext() = default;
    virtual ~ILwsCallbackContext() = default;

    ILwsCallbackContext(const ILwsCallbackContext&) = default;
    auto operator=(const ILwsCallbackContext&) -> ILwsCallbackContext& = default;

    ILwsCallbackContext(ILwsCallbackContext&&) noexcept = default;
    auto operator=(ILwsCallbackContext&&) noexcept -> ILwsCallbackContext& = default;

public:
    virtual void setStopping() = 0;
    virtual auto isStopping() const -> bool = 0;
    virtual auto getSessions() -> ILwsSessionsPtr = 0;

    virtual auto getEventHandler() -> IEventHandlerPtr = 0;
};

} // namespace ews::srv
