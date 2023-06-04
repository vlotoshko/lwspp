/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "LwsAdapter/LwsTypesFwd.hpp"

namespace ews::srv
{

class ILwsCallbackNotifier
{
public:
    ILwsCallbackNotifier() = default;
    virtual ~ILwsCallbackNotifier() = default;

    ILwsCallbackNotifier(const ILwsCallbackNotifier&) = default;
    auto operator=(const ILwsCallbackNotifier&) -> ILwsCallbackNotifier& = default;

    ILwsCallbackNotifier(ILwsCallbackNotifier&&) noexcept = default;
    auto operator=(ILwsCallbackNotifier&&) noexcept -> ILwsCallbackNotifier& = default;

public:
    virtual void sendSessionMessages(const ILwsSessionPtr&) = 0;
    virtual void sendPendingMessages() = 0;
};

} // namespace ews::srv
