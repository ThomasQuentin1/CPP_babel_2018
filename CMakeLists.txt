cmake_minimum_required(VERSION 3.7)
project(babel)

set(CMAKE_CXX_STANDARD 17)

include_directories(include .)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_GENERATOR_PLATFORM "x64")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB DLLS "${CMAKE_SOURCE_DIR}/dll/*.dll")

IF (WIN32)
    file(COPY ${DLLS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ${DLLS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
ENDIF()


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
set (CMAKE_PREFIX_PATH "C:\\Qt\\5.12.5\\msvc2017_64")
#set(BIN_MSVC_PATH "C:\\Qt\\5.12.5\\msvc2017\\bin")



find_package(Qt5Widgets CONFIG REQUIRED)

include_directories(${CMAKE_INCLUDE_PATH})

QT5_WRAP_UI(FORMS_HEADERS
    client/src/Graphic/loggedwindow.ui
    client/src/Graphic/connectiondialog.ui
)

add_executable(babel_server shared/network/WindowsInit.cpp server/src/main.cpp shared/packets.hpp shared/network/TcpConnection.cpp shared/network/TcpConnection.hpp shared/network/UdpConnection.cpp shared/network/UdpConnection.hpp shared/exceptions/NetworkException.cpp shared/exceptions/NetworkException.hpp shared/network/TcpConnection.cpp shared/network/TcpConnection.hpp shared/network/UdpConnection.cpp shared/network/UdpConnection.hpp shared/network/TcpServer.cpp shared/network/TcpServer.hpp shared/network/IPollable.cpp shared/network/IPollable.hpp shared/network/Timeout.cpp shared/network/Timeout.hpp shared/network/UdpProxy.cpp shared/network/UdpProxy.hpp shared/network/IConnection.cpp shared/network/IConnection.hpp server/src/BabelServer.cpp server/src/BabelServer.hpp server/src/Client.cpp server/src/Client.hpp server/src/singleton/Database.cpp server/src/singleton/Database.hpp shared/exceptions/IOException.cpp shared/network/ActionDyn.cpp)
add_executable(babel_client shared/network/WindowsInit.cpp shared/network/WindowsInit.hpp client/src/Graphic/connectiondialog.cpp client/src/Graphic/connectiondialog.h client/src/core.cpp client/src/core.hpp client/src/Graphic/enumcommunication.h client/src/Graphic/gui.cpp client/src/Graphic/gui.h client/src/Graphic/igui.h client/src/Graphic/loggedwindow.cpp client/src/Graphic/loggedwindow.h client/src/Graphic/user.cpp client/src/Graphic/user.h shared/exceptions/UserException.cpp shared/exceptions/UserException.hpp client/src/main.cpp client/src/Opus.cpp client/src/Opus.hpp shared/types.hpp shared/exceptions/SoundException.cpp shared/exceptions/SoundException.hpp shared/packets.hpp shared/network/TcpConnection.cpp shared/network/TcpConnection.hpp shared/network/UdpConnection.cpp shared/network/UdpConnection.hpp shared/network/TcpServer.cpp shared/network/TcpServer.hpp shared/network/IPollable.cpp shared/network/IPollable.hpp shared/network/Timeout.cpp shared/network/Timeout.hpp shared/network/UdpProxy.cpp shared/network/UdpProxy.hpp shared/network/IConnection.cpp shared/network/IConnection.hpp shared/network/ActionDyn.cpp shared/network/ActionDyn.hpp client/src/IRecorder.hpp client/src/ISpeaker.hpp client/src/ client/src/ client/src/ client/src/ client/src/audioConfig.h shared/network/TcpConnection.cpp shared/exceptions/NetworkException.cpp client/src/IEncoder.hpp client/src/Communication.cpp client/src/Communication.hpp shared/SoundPacket.cpp shared/SoundPacket.hpp shared/exceptions/UserException.hpp shared/exceptions/UserException.cpp client/src/Thread.cpp client/src/Thread.hpp client/src/PortAudio.cpp client/src/PortAudio.hpp ${FORMS_HEADERS})
add_executable(network_test shared/network/WindowsInit.cpp network_test/main.cpp shared/network/TcpConnection.cpp shared/network/UdpConnection.cpp shared/exceptions/NetworkException.cpp network_test/lowLevel.cpp network_test/console.cpp shared/network/Timeout.cpp shared/network/ActionDyn.cpp shared/SoundPacket.cpp shared/SoundPacket.hpp)
add_executable(audio_test audio_test/main.cpp client/src/ client/src/ shared/exceptions/SoundException.cpp shared/SoundPacket.cpp shared/SoundPacket.hpp client/src/Thread.cpp client/src/Thread.hpp client/src/PortAudio.cpp)
add_executable(call_test shared/network/WindowsInit.cpp shared/network/WindowsInit.hpp shared/exceptions/UserException.cpp shared/exceptions/UserException.hpp call_test/main.cpp client/src/Opus.cpp client/src/Opus.hpp shared/types.hpp shared/exceptions/SoundException.cpp shared/exceptions/SoundException.hpp shared/packets.hpp shared/network/TcpConnection.cpp shared/network/TcpConnection.hpp shared/network/UdpConnection.cpp shared/network/UdpConnection.hpp shared/network/TcpServer.cpp shared/network/TcpServer.hpp shared/network/IPollable.cpp shared/network/IPollable.hpp shared/network/Timeout.cpp shared/network/Timeout.hpp shared/network/UdpProxy.cpp shared/network/UdpProxy.hpp shared/network/IConnection.cpp shared/network/IConnection.hpp shared/network/ActionDyn.cpp shared/network/ActionDyn.hpp client/src/IRecorder.hpp client/src/ISpeaker.hpp client/src/ client/src/ client/src/ client/src/ client/src/audioConfig.h shared/network/TcpConnection.cpp shared/exceptions/NetworkException.cpp client/src/IEncoder.hpp client/src/Communication.cpp client/src/Communication.hpp shared/SoundPacket.cpp shared/SoundPacket.hpp shared/exceptions/UserException.hpp shared/exceptions/UserException.cpp client/src/Thread.cpp client/src/Thread.hpp client/src/PortAudio.cpp client/src/PortAudio.hpp)

target_link_libraries(babel_client ${CONAN_LIBS} Qt5::Widgets)
target_link_libraries(call_test ${CONAN_LIBS})
target_link_libraries(audio_test ${CONAN_LIBS})
if(WIN32)
  target_link_libraries(babel_server wsock32 ws2_32)
  target_link_libraries(babel_client wsock32 ws2_32)
  target_link_libraries(network_test wsock32 ws2_32)
  target_link_libraries(call_test wsock32 ws2_32 )

endif()

#target_link_libraries(bomberman ${Irrlicht_LIBRARIES} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
