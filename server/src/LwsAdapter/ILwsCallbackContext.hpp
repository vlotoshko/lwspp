/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "websocketpp/server/TypesFwd.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"

namespace wspp::srv
{

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

} // namespace wspp::srv
