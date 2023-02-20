/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <memory>

namespace wspp::srv
{

class IEventHandler;
using IEventHandlerPtr = std::shared_ptr<IEventHandler>;

class IMessageSender;
using IMessageSenderPtr = std::shared_ptr<IMessageSender>;

class IServer;
using IServerPtr = std::shared_ptr<IServer>;

class IServerContext;
using IServerContextPtr = std::shared_ptr<IServerContext>;

} // namespace wspp::srv
