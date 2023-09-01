#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "ImagesList.h"


#define SERVER_IP "127.0.0.1"

void send_image(int socket, const char *filename) {

    FILE* file = fopen(filename, "rb"); //AQUI HAY QUE CAMBIAR CUANDO SE PIDA EL NAME DE LA IMG

    if (file) {
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        send(socket, &file_size, sizeof(file_size), 0);

        char *image_data = (char *)malloc(file_size);
        fread(image_data, 1, file_size, file);
        fclose(file);


        // Send image to the server
        send(socket, image_data, file_size, 0);


        send(socket, "TEST", strlen("TEST")+1, 0);

        free(image_data);
    } else {
        printf("\nError seleccionando imagen\n");
    }
}

int read_Port() {
    int server_port = 1717;
    FILE *file = fopen("Sockets/config.conf", "r");
    if (file) {
        char line[128]; // Adjust the size as needed
        while (fgets(line, sizeof(line), file)) {
            // Parse lines in the format "KEY=VALUE"
            char key[64]; // Adjust the size as needed
            int value;
            if (sscanf(line, "%[^=]=%d", key, &value) == 2) {
                if (strcmp(key, "SERVER_PORT") == 0) {
                    server_port = value;
                    break; // Port found, no need to continue reading
                }
            }
        }
        fclose(file);
    }
    return server_port;
}

int main() {

    struct ImageNode *imageList = NULL;

    int SERVER_PORT = read_Port();

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error al crear el socket del cliente");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error al conectar al servidor");
        exit(EXIT_FAILURE);
    }

    printf("Conexión al servidor establecida\n");

    // Enviar una imagen al servidor
    //send_image(client_socket, "Sockets/To Send/image.png");

    addImageNode(&imageList, "Sockets/To Send/image3.png");
    addImageNode(&imageList, "Sockets/To Send/image2.png");
    addImageNode(&imageList, "Sockets/To Send/image1.png");

    printImageList(imageList);

    printf("---------------------------------\n");
    mergeSort(&imageList);
    printImageList(imageList);

    freeImageList(imageList);
    close(client_socket);

    return 0;
}