/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "easywebsocket/client/ClientFactory.hpp"

#include "IClientContext.hpp"
#include "IContextVisitor.hpp"
#include "IContextVisitorAcceptor.hpp"
#include "Client.hpp"

namespace ews::cli
{

class ClientFactory : private IContextVisitor
{
public:
    auto createClient(const IContextVisitorAcceptor& c) -> IClientPtr
    {
        c.accept(*this);
        return _client;
    }

private:
    void VisitClientContext(const ClientContext& ctx) override
    {
        _client = std::make_shared<Client>(ctx);
    }

private:
    IClientPtr _client;
};

auto createClient(const IClientContext& ctx) -> IClientPtr
{
    return ClientFactory{}.createClient(ctx);
}

} // namespace ews::cli
