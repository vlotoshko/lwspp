/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "websocketpp/server/ServerFactory.hpp"

#include "IContextVisitor.hpp"
#include "IContextVisitorAcceptor.hpp"
#include "IServerContext.hpp"
#include "Server.hpp"

namespace wspp::srv
{

class ServerFactory : private IContextVisitor
{
public:
    auto createServer(const IContextVisitorAcceptor& c) -> IServerPtr
    {
        c.accept(*this);
        return _server;
    }

private:
    void VisitServerContext(const ServerContext& ctx) override
    {
        _server = std::make_shared<Server>(ctx);
    }

private:
    IServerPtr _server;
};

auto createServer(const IServerContext& ctx) -> IServerPtr
{
    return ServerFactory{}.createServer(ctx);
}

} // namespace wspp::srv
