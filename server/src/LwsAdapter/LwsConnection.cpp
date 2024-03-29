/*
 * lwspp - C++ wrapper for the libwebsockets library
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

#include "LwsAdapter/LwsConnection.hpp"

namespace lwspp
{
namespace srv
{

namespace
{

// NOTE: Additional space with the size of LWS_PRE should be added in the front of the data
// For more information please read lws_write description
template <typename Container>
auto addPrefixToMessage(const Container& message) -> std::string
{
    std::string result;
    // NOTE: resize can throw bad alloc if message is too large
    result.resize(LWS_PRE + message.size());
    std::copy(message.cbegin(), message.cend(), &result[0] + LWS_PRE);
    return result;
}

} // namespace

LwsConnection::LwsConnection(ConnectionId connectionId, LwsInstanceRawPtr instance)
    : _connectionId(connectionId)
    , _wsInstance(instance)
{}

auto LwsConnection::getConnectionId() const -> ConnectionId
{
    return _connectionId;
}

auto LwsConnection::getLwsInstance() -> LwsInstanceRawPtr
{
    return _wsInstance;
}

void LwsConnection::addBinaryDataToSend(const std::vector<char>& binaryData)
{
    std::string payload = addPrefixToMessage(binaryData);

    const std::lock_guard<std::mutex> guard(_mutex);
    _pendingData.emplace(DataType::Binary, std::move(payload));
}

void LwsConnection::addTextDataToSend(const std::string& textData)
{
    std::string payload = addPrefixToMessage(textData);

    const std::lock_guard<std::mutex> guard(_mutex);
    _pendingData.emplace(DataType::Text, std::move(payload));
}

auto LwsConnection::getPendingData() -> std::queue<Message>&
{
    if (_pendingDataToSend.empty())
    {
        const std::lock_guard<std::mutex> guard(_mutex);
        _pendingDataToSend.swap(_pendingData);
    }
    return _pendingDataToSend;
}

auto lwspp::srv::LwsConnection::markedToClose() -> bool
{
    return _markedToClose;
}

void LwsConnection::markToClose()
{
    _markedToClose = true;
}

} // namespace srv
} // namespace lwspp
