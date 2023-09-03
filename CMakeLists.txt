cmake_minimum_required(VERSION 3.26)
project(Tarea2SO C)

set(CMAKE_C_STANDARD 11)

add_executable(Client Sockets/Client.c
        Sockets/ImagesList.c
        Sockets/ImagesList.h)
add_executable(Server Sockets/Server.c
        Sockets/Algorithm/stb_image.h
        Sockets/Algorithm/stb_image_write.h
        Sockets/Algorithm/equalization.h
        Sockets/Algorithm/image_handler.h
        Sockets/Algorithm/equalization.c
        Sockets/Algorithm/image_handler.c
        )

target_link_libraries(Server PRIVATE m)