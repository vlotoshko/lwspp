/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <memory>

namespace ews::srv
{

class IEventHandler;
using IEventHandlerPtr = std::shared_ptr<IEventHandler>;

class IMessageSender;
using IMessageSenderPtr = std::shared_ptr<IMessageSender>;

class IMessageSenderAcceptor;
using IMessageSenderAcceptorPtr = std::shared_ptr<IMessageSenderAcceptor>;

class IServer;
using IServerPtr = std::shared_ptr<IServer>;

class IServerContext;
using IServerContextPtr = std::shared_ptr<IServerContext>;

class ISessionInfo;
using ISessionInfoPtr = std::shared_ptr<ISessionInfo>;

} // namespace ews::srv
