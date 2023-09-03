/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include "easywebsocket/client/TypesFwd.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"

namespace ews::cli
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

    virtual auto getSession() -> ILwsSessionPtr = 0;
    virtual void setSession(ILwsSessionPtr) = 0;
    virtual void resetSession() = 0;

    virtual auto getEventHandler() -> IEventHandlerPtr = 0;
};

} // namespace ews::cli
