/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include "websocketpp/server/IServer.hpp"
#include "websocketpp/server/ServerFactory.hpp"
#include "websocketpp/server/ServerContextBuilder.hpp"
#include "websocketpp/server/Types.hpp"

#include "EventHandler.hpp"

auto main() -> int
{
    using namespace wspp;
    const srv::Port PORT = 9000;

    auto serverContextBuilder = srv::ServerContextBuilder{};
    serverContextBuilder
            .setPort(PORT)
            .setVersion(srv::ServerVersion::v1_Andromeda)
            .setEventHandler(std::make_shared<chat::EventHandler>())
            .setLogging(1)
            ;

    auto server = srv::createServer(*serverContextBuilder.build());
    // blocks the thread
    server->start();

    return 0;
}
