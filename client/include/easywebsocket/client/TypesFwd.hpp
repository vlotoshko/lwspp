/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <memory>

namespace ews::cli
{

class IClient;
using IClientPtr = std::shared_ptr<IClient>;

class IClientContext;
using IClientContextPtr = std::shared_ptr<IClientContext>;

class IEventHandler;
using IEventHandlerPtr = std::shared_ptr<IEventHandler>;

class IMessageSender;
using IMessageSenderPtr = std::shared_ptr<IMessageSender>;

class IMessageSenderAcceptor;
using IMessageSenderAcceptorPtr = std::shared_ptr<IMessageSenderAcceptor>;

class ISessionInfo;
using ISessionInfoPtr = std::shared_ptr<ISessionInfo>;

} // namespace ews::cli
