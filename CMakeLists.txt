cmake_minimum_required(VERSION 3.26)
project(Tarea2SO C)

set(CMAKE_C_STANDARD 11)

add_executable(Client Sockets/Client.c
        Sockets/ImagesList.c
        Sockets/ImagesList.h)
add_executable(Server Sockets/Server.c)
