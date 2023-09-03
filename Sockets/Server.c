#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>



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



char* read_dir(const char *key) {
    char *value = NULL;

    FILE *file = fopen("Sockets/config.conf", "r");
    if (file) {
        char line[128];
        while (fgets(line, sizeof(line), file)) {
            char file_key[128];
            char file_value[128];

            // Parse lines in the format "KEY=VALUE"
            if (sscanf(line, "%[^=]=%s", file_key, file_value) == 2) {
                if (strcmp(file_key, key) == 0) {
                    value = strdup(file_value);
                    break; // Key found, no need to continue reading
                }
            }
        }
        fclose(file);
    }

    return value;
}


void save_image(const char *filename, const char *image_data, size_t data_size) {
    // Create the "Received" folder if it doesn't exist
    mkdir("Sockets/Received", 0777);


    // Construct the full path to the image file within the "Received" folder
    char full_path[128]; // Adjust the size as needed
    snprintf(full_path, sizeof(full_path), "Sockets/Received/%s", filename);

    FILE *file = fopen(full_path, "wb");
    if (file) {
        fwrite(image_data, 1, data_size, file);
        fclose(file);
        printf("Saved image as %s\n", full_path);
    } else {
        perror("Error saving image");
    }
}

void *console_input(void *arg) {
    char input[10];
    while (1) {
        printf("Enter 'STOP' to stop the server, 'RESTART' to restart the server, or 'STATUS' to check the server: ");
        scanf("%s", input);

        if (strcmp(input, "STOP") == 0) {
            printf("Stopping the server...\n");
            exit(0);

        }if (strcmp(input, "RESTART") == 0) {
            //TENGO QUE VER COMO PUTAS BOTARLO Y LEVANTARLO DE NUEVO

        }if (strcmp(input, "STATUS") == 0) {
            printf("Server running, listening to port %d\n", read_Port());

        }
    }
}

int main() {

    printf("Type 'START' to start the server, otherwise the program will be closed.\n");

    int SERVER_PORT = read_Port();

    char input[10];
    scanf("%s", input);

    if (strcmp(input, "START") != 0) {
        printf("Stopping the server...\n");
        exit(0);
    }

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating server socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding server socket");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) == -1) {
        perror("Error listening on server socket");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", SERVER_PORT);

    pthread_t input_thread;
    if (pthread_create(&input_thread, NULL, console_input, NULL) != 0) {
        perror("Error creating input thread");
        exit(EXIT_FAILURE);
    }


    while (1) {

        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1) {
            perror("Error accepting client connection");
            continue;
        }

        printf("Client connected\n");

        // Recibir tamaño de la imagen
        long file_size;
        ssize_t bytes_received = recv(client_socket, &file_size, sizeof(long), 0);

        if (bytes_received > 0) {


            printf("RECEIVED FILE SIZE: %ld\n", file_size);

            if(bytes_received < 0){
                perror("ERROR: COULD NOT RECEIVE FILE SIZE\n");
                exit(EXIT_FAILURE);
            }

            // Recibir imagen
            unsigned char *image_data = (unsigned char *)malloc(file_size);
            bytes_received = recv(client_socket, image_data, file_size, 0);

            if(bytes_received < 0){
                perror("ERROR: COULD NOT RECEIVE FILE\n");
                exit(EXIT_FAILURE);
            }

            char filename[1024];
            bytes_received = recv(client_socket, filename, sizeof(filename), 0);

            filename[bytes_received] = '\0';

            //imprimir nombre de la imagen
            printf("FILENAME: %s\n", filename);



            //Salvar imagen en server/images/filename
            save_image(filename, image_data, file_size);
            //printf("IMAGE SAVED\n");

            free(image_data);
        }

        close(client_socket);
    }


    close(server_socket);
    printf("Server closed\n");


    return 0;
}
