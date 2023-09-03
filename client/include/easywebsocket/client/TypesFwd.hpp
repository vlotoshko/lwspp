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

class IDataSender;
using IDataSenderPtr = std::shared_ptr<IDataSender>;

class IDataSenderAcceptor;
using IDataSenderAcceptorPtr = std::shared_ptr<IDataSenderAcceptor>;

class IEventHandler;
using IEventHandlerPtr = std::shared_ptr<IEventHandler>;

class ISessionInfo;
using ISessionInfoPtr = std::shared_ptr<ISessionInfo>;

class SslSettings;
using SslSettingsPtr = std::shared_ptr<SslSettings>;

} // namespace ews::cli
