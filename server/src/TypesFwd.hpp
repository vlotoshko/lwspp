/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <memory>

namespace ews::srv
{

class IContextVisitor;
class IContextVisitorAcceptor;
class ServerContext;

class LwsContext;
using LwsContextPtr = std::shared_ptr<LwsContext>;

} // namespace ews::srv
