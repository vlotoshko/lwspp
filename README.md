# lwspp: C++ libraries for WebSocket protocol support

lwspp provides high-level abstractions for both client and server implementations of the WebSocket protocol. This allows users to easily integrate WebSocket functionality into their C++ projects. 

lwspp serves as a wrapper for the [libwebsockets](https://libwebsockets.org/) library.

lwspp is divided into two main libraries:

- **lwspp-client**: A C++ library that simplifies WebSocket client implementation.
- **lwspp-server**: A C++ library that simplifies WebSocket server implementation.

Users can choose the library that suits their needs based on whether they are developing a WebSocket client or server application.

<p align="center">
  <a href='https://github.com/vlotoshko/lwspp/releases'>
    <img alt="Release" src="https://img.shields.io/github/v/release/vlotoshko/lwspp?label=version&style=for-the-badge">
  </a>  
<!--   <a href='https://github.com/vlotoshko/lwspp/releases'>
    <img alt="Downloads" src="https://img.shields.io/github/downloads/vlotoshko/lwspp/total?style=for-the-badge&logo=github">
  </a> -->
  <a href="LICENSE">
    <img alt="License" src="https://img.shields.io/github/license/vlotoshko/lwspp?style=for-the-badge">
  </a>
</p>

<p align="center">
  <a href="#getting-started">Getting Started</a> •
  <a href="#usage">Usage</a> •
  <a href="#examples">Examples</a> •
  <a href="#operating-systems">Operating Systems</a> •
  <a href="#contributing">Contributing</a> •
  <a href="#license">License</a>
</p>

## Getting Started

### Prerequisites

lwspp depends on the following library:

- [libwebsockets](https://libwebsockets.org/) - the core WebSocket library written in C.

Follow the installation instructions for libwebsockets to ensure it is properly installed on your system.

Before building the lwspp project with the _WEBSOCKETS_GITHUB_TAG_ option, please ensure that the following prerequisites are met:

  - Git - must be installed on your system to download the libwebsockets library from GitHub.

### Installation

The installation steps for integrating lwspp into your C++ project are as follows:

1. Clone the lwspp repository:

   ```sh
   git clone https://github.com/vlotoshko/lwspp.git
   ```

2. Navigate to the lwspp project directory:
   ```sh
   cd lwspp
   ```

3. Create a directory for building and change to it:
   ```sh
   mkdir build
   cd build
   ```
4. Configure project and build lwspp:

   ```sh
   cmake ..
   make
   ```

5. Finally, install lwspp into the system:
   ```sh
   sudo make install
   ```
These steps should help you integrate lwspp into your C++ project successfully.

#### Compilation Options

lwspp offers a variety of compilation options to tailor the build process according to your specific requirements. You can configure these options using CMake when generating the build system for your project. Below are the available compilation options:

- **Build lwspp Client Library** (`OPTION_BUILD_CLIENT`): Enable or disable the build of the lwspp client library. (Default: **ON**)

- **Build lwspp Server Library** (`OPTION_BUILD_SERVER`): Enable or disable the build of the lwspp server library. (Default: **ON**)

- **Build Shared Libraries** (`OPTION_BUILD_SHARED`): Enable or disable the build of shared lwspp libraries. (Default: **ON**)

- **Build Static Libraries** (`OPTION_BUILD_STATIC`): Enable or disable the build of static lwspp libraries. (Default: **ON**)

- **Build Example Applications** (`OPTION_BUILD_EXAMPLES`): Enable or disable the build of example applications. (Default: **OFF**)

- **Build Integration Tests Application** (`OPTION_BUILD_INTEGRATION_TESTS`): Enable or disable the build of the integration tests application. (Default: **OFF**)

- **Install Tests Directory** (`TESTS_INSTALL_DIR`): Specify the directory where the tests should be installed. If you set this option, it will add the target  **install-lwspp-tests**. (Default: **""**)

- **Install Examples Directory** (`EXAMPLES_INSTALL_DIR`): Specify the path where examples should be installed after the build. Setting this option adds the corresponding **install-'project name'** target for each example. (Default: **""**)

- **Websockets Search Directory** (`WEBSOCKETS_SEARCH_DIR`): If set, the libwebsockets library located in the corresponding directory will be used. (Default: **""**)

- **Websockets Library Path** (`WEBSOCKETS_LIBRARY`): Shows the path to the libwebsockets library that will be linked when building the project. This variable helps you identify the version of the library currently in use.

You can configure these options using CMake's -D flag when generating your project's build system. For example, to enable the building and installing of example applications, you can use:
   ```bash
   cmake -D OPTION_BUILD_EXAMPLES=ON -D EXAMPLES_INSTALL_DIR=./bin ..
   make install-lwspp-binance install-lwspp-chat install-lwspp-helloworld
   ```
After running the _'make'_ command, the example applications will be built and installed in the _'./bin'_ directory.

Please be aware that the installed binaries of the example applications and tests depend on the lwspp libraries being installed in your system (as outlined in step 5 of the installation guide).

## Usage

This project provides distinct include directories for client and server integration, namely 'lwspp/client/' (using the 'cli' namespace) and 'lwspp/server/' (using the 'srv' namespace), respectively. You can configure and create both the client and server using their respective builders. Upon construction, both the client and server initiate connections/listening, and upon destruction, they gracefully terminate connections/stop listening. They each operate in a separate thread to manage connections and data transfers.

### Server Configuration

To set up and create a WebSocket server, you'll use the Server Builder, located at lwspp/server/ServerBuilder.hpp. The Server Builder has several mandatory options that must be configured before building the server; otherwise, it will throw an exception.

The mandatory options for the server builder include:

1. **Port**: Specify the port on which the server will listen.
2. **Callback Version**: The libwebsockets library requires a specific callback to handle events and data. lwspp provides this callback. To ensure backward compatibility, you can set the previous callback to maintain the same behavior.
3. **ServerControl Acceptor**: When constructing the server, the server builder provides an IServerControl instance. IServerControl is used to perform actions defined by the the IServerControl interface, for example sending data from server to a client. To obtain this IServerControl, implement the IServerControlAcceptor interface. Users of the library should implement this interface to manage data sent from the server to clients.
4. **ServerLogic**: The ServerLogic defines the server's behavior. You should implement the desired server behavior using the IServerLogic interface.

For most use cases, the **ServerLogicBase** class can be a convenient starting point for implementing your event ServerLogic. This class not only implements the _IServerLogic_ interface, which includes stubs for all its methods but also implements the _IServerControlAcceptor_ interface. This means you can easily obtain the ServerControl required for performing actions defined by the IServerControl interface.

The **ServerLogicBase** class provides a basic framework for event handling and data transmission. You can extend and customize its behavior to suit your specific server requirements. This approach saves you time and effort when developing WebSocket server functionality.

### Client Configuration

The client configuration closely resembles the server configuration. As mentioned earlier, it is defined in a separate include directory 'lwspp/client/' and uses the 'cli' namespace. Additionally, it requires an additional mandatory option - **address**.

### More Information

For more detailed usage instructions and insights, refer to the [examples](examples), [test cases](tests), or header file descriptions.

## Examples

### The Hello World example

In this straightforward example, both the client and server are configured and constructed. The client's behavior involves sending the message 'hello server' upon connection, while the server's behavior includes sending the message 'hello client' upon receiving a message from a client.
```cpp
#include <iostream>
#include <thread>

#include "lwspp/client/ClientBuilder.hpp"
#include "lwspp/client/ClientLogicBase.hpp"
#include "lwspp/client/IClientControl.hpp" // IWYU pragma: keep

#include "lwspp/server/IServerControl.hpp" // IWYU pragma: keep
#include "lwspp/server/ServerBuilder.hpp"
#include "lwspp/server/ServerLogicBase.hpp"

using namespace lwspp;

class ClientLogic : public cli::ClientLogicBase
{
public:
    void onConnect(cli::IConnectionInfoPtr) noexcept override
    {
        _clientControl->sendTextData("hello server!");
    }

    void onTextDataReceive(const cli::DataPacket& dataPacket) noexcept override
    {
        std::cout << "client received the message: " << std::string{dataPacket.data, dataPacket.length} << std::endl;
    }
};

class ServerLogic : public srv::ServerLogicBase
{
public:
    void onTextDataReceive(srv::ConnectionId, const srv::DataPacket& dataPacket) noexcept override
    {
        std::cout << "server received the message: " << std::string{dataPacket.data, dataPacket.length}  << std::endl;
        _serverControl->sendTextData("hello client!");
    }
};

auto main() -> int
{
    const srv::Port PORT = 9000;

    // Configure and build server
    auto serverLogic = std::make_shared<ServerLogic>();
    auto serverBuilder = srv::ServerBuilder{};
    serverBuilder
        .setPort(PORT)
        .setCallbackVersion(srv::CallbackVersion::v1_Andromeda)
        .setServerLogic(serverLogic)
        .setServerControlAcceptor(serverLogic)
        .setLwsLogLevel(0)
        ;
    auto server = serverBuilder.build();

    // Configure and build client
    auto clientLogic = std::make_shared<ClientLogic>();
    auto clientBuilder = cli::ClientBuilder{};
    clientBuilder
        .setAddress("localhost")
        .setPort(PORT)
        .setCallbackVersion(cli::CallbackVersion::v1_Amsterdam)
        .setClientLogic(clientLogic)
        .setClientControlAcceptor(clientLogic)
        .setLwsLogLevel(0)
        ;
    auto client = clientBuilder.build();

    // Give some time for the client and server to exchange messages.
    const auto timeout = 100U;
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));

    return 0;
}
```
### Examples directory

In the [examples](examples) directory, you can find the source code for some sample applications:

1. **binance**: This example represents a client that establishes a connection to the Binance server using SSL, receives a few messages from the server and then disconnects.
2. **chat**: A more complex example that consists of a simple HTML-based client and a chat server. The chat server supports the following features:
   - user nicknames
   - private and public messages
   - message history
3. **helloworld**: This is the previously mentioned "Hello World" [example](#the-hello-world-example).

## Operating Systems

The lwspp project was built and tested on the following operating systems up to _September 29, 2023_:
  - [x] Ubuntu 24.04 LTS
  - [x] Red Hat Enterprise Linux (RHEL) 9.2 
    - The path '/usr/local/lib' was appended to the LD_LIBRARY_PATH to enable the system to locate the lwspp library.
  - [x] openSUSE Leap 15.4
  - [x] macOS Ventura 13.4
    - The libwebsockets library was installed using MacPorts.

Please note that compatibility with newer operating system versions or different library versions may vary.

## Contributing

All contributions are **greatly appreciated**.

## License

Distributed under the MIT License. See [LICENSE](LICENSE) for more information.
