/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include "easywebsocket/server/IServer.hpp"
#include "easywebsocket/server/ServerFactory.hpp"
#include "easywebsocket/server/ServerContextBuilder.hpp"
#include "easywebsocket/server/Types.hpp"

#include "EventHandler.hpp"

auto main() -> int
{
    using namespace ews;
    const srv::Port PORT = 9000;

    auto serverContextBuilder = srv::ServerContextBuilder{};
    serverContextBuilder
            .setPort(PORT)
            .setVersion(srv::ServerVersion::v1_Andromeda)
            .setEventHandler(std::make_shared<chat::EventHandler>())
            ;

    auto server = srv::createServer(*serverContextBuilder.build());

    // blocks the thread
    server->start();

    return 0;
}
