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

#include <csignal>
#include <thread>

#include "lwspp/server/ServerBuilder.hpp"
#include "lwspp/server/Types.hpp"
#include "lwspp/server/TypesFwd.hpp"

#include "ServerLogic.hpp"

volatile std::sig_atomic_t stopRequested = 0;

extern "C" void signalHandler(int)
{
     stopRequested = 1;
}

void waitForSignal()
{
    while (stopRequested == 0)
    {
        const int sleepTimeout = 100;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeout));
    }
}

auto main() -> int
{
    signal(SIGINT, signalHandler);

    using namespace lwspp;
    const srv::Port PORT = 9000;

    auto serverLogic = std::make_shared<chat::ServerLogic>();
    auto serverBuilder = srv::ServerBuilder{};
    serverBuilder
        .setPort(PORT)
        .setCallbackVersion(srv::CallbackVersion::v1_Andromeda)
        .setServerLogic(serverLogic)
        .setServerControlAcceptor(serverLogic)
        ;
    auto server = serverBuilder.build();

    waitForSignal(); // Ctrl+C

    return 0;
}
