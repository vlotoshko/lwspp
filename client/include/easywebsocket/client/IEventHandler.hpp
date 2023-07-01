/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Apr, 2023
 */

#pragma once

#include <string>

#include "easywebsocket/client/TypesFwd.hpp"

namespace ews::cli
{

/**
 * @brief The IEventHandler class defines interface for the client behavior implementation.
 * The library user implements this interface on its own.
 */
class IEventHandler
{
public:
    IEventHandler() = default;
    virtual ~IEventHandler() = default;

    IEventHandler(IEventHandler&&) = default;
    auto operator=(IEventHandler&&) noexcept -> IEventHandler& = default;

    IEventHandler(const IEventHandler&) = delete;
    auto operator=(const IEventHandler&) noexcept -> IEventHandler& = delete;

public:
    virtual void onConnect(ISessionInfoPtr) noexcept = 0;
    virtual void onDisconnect() noexcept = 0;
    virtual void onMessageReceive(const std::string& message) noexcept = 0;

    virtual void onError(const std::string& errorMessage) noexcept = 0;
    virtual void onWarning(const std::string& errorMessage) noexcept = 0;
};

} // namespace ews::cli
