/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include <libwebsockets.h>
#include <memory>
#include <vector>

namespace wspp::cli
{

class ILwsCallbackContext;
using ILwsCallbackContextPtr = std::shared_ptr<ILwsCallbackContext>;

class ILwsSession;
using ILwsSessionPtr = std::shared_ptr<ILwsSession>;
using ILwsSessionWeak = std::weak_ptr<ILwsSession>;

struct LwsDataHolder;
using LwsDataHolderPtr = std::shared_ptr<LwsDataHolder>;

using LwsInstanceRawPtr = lws*;
using LowLevelContextPtr = std::shared_ptr<lws_context>;

using LwsProtocols = std::vector<lws_protocols>;
using LwsCallback = lws_callback_function;
using LwsConnectInfo = lws_client_connect_info;

} // namespace wspp::cli
