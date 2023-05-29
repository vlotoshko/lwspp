/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include "websocketpp/client/TypesFwd.hpp"

namespace wspp::cli
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
    virtual auto getEventHandler() -> IEventHandlerPtr = 0;
};

} // namespace wspp::cli
