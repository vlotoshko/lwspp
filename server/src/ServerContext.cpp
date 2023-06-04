/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "IContextVisitor.hpp"
#include "ServerContext.hpp"

namespace ews::srv
{

void ServerContext::accept(IContextVisitor& visitor) const
{
    visitor.VisitServerContext(*this);
}


} // namespace ews::srv
