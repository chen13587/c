cmake_minimum_required(VERSION 3.12)
project(udp_mysql)

set(CMAKE_CXX_STANDARD 14)
link_libraries(libmysqlclient.so)
add_executable(udp_mysql main.cpp udp_server.cpp mysql_server.cpp mysql_server.h udp_server.h tool.cpp tool.h)