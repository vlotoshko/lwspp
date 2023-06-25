/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include <condition_variable>
#include <csignal>

#include "easywebsocket/server/ServerFactory.hpp"
#include "easywebsocket/server/ServerContextBuilder.hpp"
#include "easywebsocket/server/Types.hpp"
#include "easywebsocket/server/TypesFwd.hpp"

#include "EventHandler.hpp"

namespace interruption
{

std::mutex mutex;
std::condition_variable condVar;
bool gotSignal = false;

void signalHandler(int)
{
    gotSignal = true;
    condVar.notify_one();
}

void waitForSignal()
{
    std::unique_lock<std::mutex> guard(mutex);
    condVar.wait(guard, []{ return gotSignal; });
}

} // namespace name

auto main() -> int
{
    signal(SIGINT, interruption::signalHandler);

    using namespace ews;
    const srv::Port PORT = 9000;

    auto serverContextBuilder = srv::ServerContextBuilder{};
    serverContextBuilder
            .setPort(PORT)
            .setVersion(srv::ServerVersion::v1_Andromeda)
            .setEventHandler(std::make_shared<chat::EventHandler>())
            ;
    auto server = srv::createServer(*serverContextBuilder.build());

    interruption::waitForSignal();
    return 0;
}
