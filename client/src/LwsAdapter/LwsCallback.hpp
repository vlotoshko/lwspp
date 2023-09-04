/*
 * EasyWebsockets - C++ wrapper for the libwebsockets library
 *
 * Copyright (C) 2023 - 2023 Volodymyr Lotoshko <vlotoshko@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#include <libwebsockets.h>

namespace ews::cli
{

/**
 * @brief lwsCallback_v1 is the LwsCallback function that defines the core behavoir of the client.
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

} // namespace ews::cli
