/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <string>

#include "easywebsocket/client/TypesFwd.hpp"
#include "Consts.hpp"
#include "IClientContext.hpp"

namespace ews::cli
{

class ClientContext : public IClientContext
{
public:
    void accept(IContextVisitor&) const override;

    ClientVersion clientVersion = UNDEFINED_CLIENT_VERSION;
    Address address = UNDEFINED_ADDRESS;
    Port port = UNDEFINED_PORT;
    Path path = UNDEFINED_PATH;
    IEventHandlerPtr eventHandler;
};

} // namespace ews::cli
