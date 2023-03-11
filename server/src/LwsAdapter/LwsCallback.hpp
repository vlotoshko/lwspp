/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include <libwebsockets.h>

namespace wspp::srv
{

/**
 * @brief lwsCallback_v1 is the LwsCallback function that defines the core behavoir of the server.
 * @param wsi - Opaque websocket instance pointer
 * @param reason - The reason for the call
 * @param userData - Pointer to per-session user data allocated by library
 * @param in - Pointer used for some callback reasons
 * @param size - Length set for some callback reasons
 * @return some int, the value of this int is not documented
 *
 * @link https://libwebsockets.org/lws-api-doc-main/html/group__usercb.html#gad4fcb82e68d60ffacca61a3f783a0a2f
 */
auto lwsCallback_v1(
        lws *wsi,
        lws_callback_reasons reason,
        void* userData,
        void* in,
        size_t len)
-> int;

} // namespace wspp::srv
