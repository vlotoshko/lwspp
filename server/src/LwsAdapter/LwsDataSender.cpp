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

#include "LwsAdapter/ILwsCallbackNotifier.hpp"
#include "LwsAdapter/ILwsSession.hpp"
#include "LwsAdapter/ILwsSessions.hpp"
#include "LwsAdapter/LwsDataSender.hpp"

namespace lwspp
{
namespace srv
{

LwsDataSender::LwsDataSender(ILwsSessionsPtr s, ILwsCallbackNotifierPtr n)
    : _sessions(std::move(s))
    , _notifier(std::move(n))
{}

void LwsDataSender::sendTextData(SessionId sessionId, const std::string& message)
{
    if (auto session = _sessions->get(sessionId))
    {
        session->addTextDataToSend(message);
        _notifier->notifyPendingDataAdded(session);
    }
}

void LwsDataSender::sendBinaryData(SessionId sessionId, const std::vector<char>& data)
{
    if (auto session = _sessions->get(sessionId))
    {
        session->addBinaryDataToSend(data);
        _notifier->notifyPendingDataAdded(session);
    }
}

void LwsDataSender::sendTextData(const std::string& message)
{
    for(auto& entry : _sessions->getAllSessions())
    {
        if (entry != nullptr)
        {
            entry->addTextDataToSend(message);
        }
        else
        {
            // TODO: log warning
        }
    }
    _notifier->notifyPendingDataAdded();
}

void LwsDataSender::sendBinaryData(const std::vector<char>& data)
{
    for(auto& entry : _sessions->getAllSessions())
    {
        if (entry != nullptr)
        {
            entry->addBinaryDataToSend(data);
        }
        else
        {
            // TODO: log warning
        }
    }
    _notifier->notifyPendingDataAdded();
}

} // namespace srv
} // namespace lwspp
