/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <memory>

namespace wspp::cli
{

class IClient;
using IClientPtr = std::shared_ptr<IClient>;

class IClientContext;
using IClientContextPtr = std::shared_ptr<IClientContext>;

} // namespace wspp::cli
