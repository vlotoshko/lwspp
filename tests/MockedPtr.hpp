/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#pragma once

#include "fakeit.hpp"

namespace ews::tests
{

template<typename Interface>
class MockedPtr
{
public:
    MockedPtr()
        : _ptr(&_mocked.get(), [](Interface*){})
    {}

    auto mock() -> fakeit::Mock<Interface>&
    {
        return _mocked;
    }

    auto ptr() -> std::shared_ptr<Interface>
    {
        return _ptr;
    }

private:
    fakeit::Mock<Interface> _mocked;
    std::shared_ptr<Interface> _ptr;
};

} // namespace ews::tests
