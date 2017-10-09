cmake_minimum_required(VERSION 3.8)
project(V_SYS_TWMailer)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES_CLIENT MyClient/myclient.cpp)
add_executable(V_SYS_TWMailer_CLIENT ${SOURCE_FILES_CLIENT})

set(SOURCE_FILES_SERVER MyServer/myserver.cpp)
add_executable(V_SYS_TWMailer_SERVER ${SOURCE_FILES_SERVER})
