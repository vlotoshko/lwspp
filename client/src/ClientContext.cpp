/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "IContextVisitor.hpp"
#include "ClientContext.hpp"

namespace wspp::cli
{

void ClientContext::accept(IContextVisitor& visitor) const
{
    visitor.VisitClientContext(*this);
}

} // namespace wspp::cli
