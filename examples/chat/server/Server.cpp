/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#include "websocketpp/server/IServer.hpp"
#include "websocketpp/server/ServerFactory.hpp"

#include "EventHandler.hpp"
#include "Server.hpp"

namespace wspp::chat
{

//Server::Server(const srv::IServerContext& context)
//    : _serverImpl(createServer(context))
//    , _eventHandler(std::make_shared<EventHandler>(_serverImpl->getMessageSender()))
//{

//}

//void Server::start()
//{
//    _serverImpl->start();
//}

//void Server::stop()
//{
//    _serverImpl->stop();
//}

} // namespace wspp::chat
