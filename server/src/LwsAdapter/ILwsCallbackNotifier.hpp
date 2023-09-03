/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "LwsAdapter/LwsTypesFwd.hpp"

namespace ews::srv
{

/**
 * @brief The ILwsCallbackNotifier class notifies the lws service that there are
 * pending data to send
 */
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
    // Notifies that pending data was added to send to the specific session.
    virtual void notifyPendingDataAdded(const ILwsSessionPtr&) = 0;
    // Notifies that pending data was added to send to the all sessions.
    virtual void notifyPendingDataAdded() = 0;
};

} // namespace ews::srv
