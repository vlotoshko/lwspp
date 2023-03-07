/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <libwebsockets.h>
#include <memory>
#include <vector>


namespace wspp::srv
{

class ILwsCallbackContext;
using ILwsCallbackContextPtr = std::shared_ptr<ILwsCallbackContext>;

class ILwsCallbackNotifier;
using ILwsCallbackNotifierPtr = std::shared_ptr<ILwsCallbackNotifier>;

class ILwsSession;
using ILwsSessionPtr = std::shared_ptr<ILwsSession>;

class ILwsSessions;
using ILwsSessionsPtr = std::shared_ptr<ILwsSessions>;

using LwsInstanceRawPtr = lws*;

struct LwsDataHolder;
using LwsDataHolderPtr = std::shared_ptr<LwsDataHolder>;
using LwsDataHolderWeak = std::weak_ptr<LwsDataHolder>;

using LowLevelContextPtr = std::shared_ptr<lws_context>;
using LowLevelContextWeak = std::weak_ptr<lws_context>;

using LwsProtocols = std::vector<lws_protocols>;
using LwsCallback = lws_callback_function;

} // namespace wspp::srv
