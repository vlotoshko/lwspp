/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "IContextVisitor.hpp"
#include "ClientContext.hpp"

namespace ews::cli
{

void ClientContext::accept(IContextVisitor& visitor) const
{
    visitor.VisitClientContext(*this);
}

} // namespace ews::cli
