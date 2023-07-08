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

class IEventHandler;
using IEventHandlerPtr = std::shared_ptr<IEventHandler>;

class IMessageSender;
using IMessageSenderPtr = std::shared_ptr<IMessageSender>;

class IMessageSenderAcceptor;
using IMessageSenderAcceptorPtr = std::shared_ptr<IMessageSenderAcceptor>;

class ISessionInfo;
using ISessionInfoPtr = std::shared_ptr<ISessionInfo>;

class SslSettings;
using SslSettingsPtr = std::shared_ptr<SslSettings>;

} // namespace ews::cli
