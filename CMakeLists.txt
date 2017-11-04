cmake_minimum_required(VERSION 3.8)
project(V_SYS_TWMailer)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES_CLIENT MyClient/myclient.cpp)
add_executable(V_SYS_TWMailer_CLIENT ${SOURCE_FILES_CLIENT})

set(SOURCE_FILES_SERVER MyServer/myserver.cpp MyServer/ServerOperation.cpp MyServer/ServerOperation.h MyServer/DeleteMessage.cpp MyServer/DeleteMessage.h MyServer/ReadMessage.cpp MyServer/ReadMessage.h MyServer/ListMessage.cpp MyServer/ListMessage.h MyServer/SendMessage.cpp MyServer/SendMessage.h MyServer/ClientHandler.cpp MyServer/ClientHandler.h MyServer/mySocket.cpp MyServer/mySocket.h MyServer/LdapLogin.cpp MyServer/LdapLogin.h MyServer/User.cpp MyServer/User.h MyServer/Quit.cpp MyServer/Quit.h MyServer/TimeOutManager.h MyServer/TimeOutManager.cpp)
set(GCC_FLAGS "-pthread -lldap -DLDAP_DEPRECATED -luuid")
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} ${GCC_FLAGS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_FLAGS}")
add_executable(V_SYS_TWMailer_SERVER ${SOURCE_FILES_SERVER})
target_link_libraries(V_SYS_TWMailer_SERVER /usr/lib/x86_64-linux-gnu/libldap.so)
target_link_libraries(V_SYS_TWMailer_SERVER /usr/lib/x86_64-linux-gnu/liblber.so  -luuid)