/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "websocketpp/server/TypesFwd.hpp"
#include "TypesFwd.hpp"

namespace wspp::srv
{

class ICallbackContext
{
public:
    ICallbackContext() = default;
    virtual ~ICallbackContext() = default;

    ICallbackContext(const ICallbackContext&) = default;
    auto operator=(const ICallbackContext&) -> ICallbackContext& = default;

    ICallbackContext(ICallbackContext&&) noexcept = default;
    auto operator=(ICallbackContext&&) noexcept -> ICallbackContext& = default;

public:
    virtual void setServerStopped() = 0;
    virtual auto isServerStopped() const -> bool = 0;
    virtual auto getEventHandler() -> IEventHandlerPtr = 0;
    virtual auto getSessions() -> ISessionsPtr = 0;
};

} // namespace wspp::srv
