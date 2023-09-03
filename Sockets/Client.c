#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "ImagesList.h"

#define SERVER_IP "127.0.0.1"





void send_image(int socket, const char *filename) {

    char full_path[128]; // Adjust the size as needed
    snprintf(full_path, sizeof(full_path), "Sockets/To Send/%s", filename);
    FILE* file = fopen(full_path, "rb"); //AQUI HAY QUE CAMBIAR CUANDO SE PIDA EL NAME DE LA IMG


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


        send(socket, filename, strlen(filename)+1, 0);

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

void sendInformation (const char *name) {
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

    send_image(client_socket, name);

    close(client_socket);
}

int main() {
    int flag = 1;
    char input[20];
    struct ImageNode *imageList = NULL;

    while (flag) {
        printf("Type the name of the image you want to send. Enter 'EXIT' to send the images to the server.\n" );
        scanf("%s", input);

        if (strcmp(input, "EXIT") == 0) {
            printf("Sending images\n");
            mergeSort(&imageList);
            flag = 0;
        } else {
            addImageNode(&imageList, input);
        }
    }

    while (imageList != NULL) {
        sendInformation(imageList->name);
        imageList = imageList->next;
    }

    freeImageList(imageList);

    return 0;
}



