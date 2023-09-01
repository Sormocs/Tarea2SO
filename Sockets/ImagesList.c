#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ImagesList.h"

struct ImageNode *createImageNode(const char *name, long file_size, const unsigned char *data) {
    struct ImageNode *newNode = (struct ImageNode *)malloc(sizeof(struct ImageNode));
    if (newNode == NULL) {
        perror("Error allocating memory for image node");
        exit(EXIT_FAILURE);
    }

    strncpy(newNode->name, name, sizeof(newNode->name));
    newNode->file_size = file_size;
    newNode->data = (unsigned char *)malloc(file_size);
    if (newNode->data == NULL) {
        perror("Error allocating memory for image data");
        free(newNode);
        exit(EXIT_FAILURE);
    }
    memcpy(newNode->data, data, file_size);
    newNode->next = NULL;

    return newNode;
}

void addImageNode(struct ImageNode **head, const char *name) {

    long file_size;
    char *image_data;
    FILE* file = fopen(name, "rb"); //AQUI HAY QUE CAMBIAR CUANDO SE PIDA EL NAME DE LA IMG

    if (file) {
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        image_data = (char *)malloc(file_size);
        fread(image_data, 1, file_size, file);
        fclose(file);

    } else {
        printf("\nError seleccionando imagen\n");
    }

    struct ImageNode *newNode = createImageNode(name, file_size, image_data);


    if (*head == NULL) {
        *head = newNode;
    } else {
        struct ImageNode *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void freeImageList(struct ImageNode *head) {
    while (head != NULL) {
        struct ImageNode *temp = head;
        head = head->next;
        free(temp->data);
        free(temp);
    }
}

void printImageList(struct ImageNode *head) {
    struct ImageNode *current = head;

    while (current != NULL) {
        printf("Image Name: %s\n", current->name);
        printf("File Size: %zu bytes\n", current->file_size);
        //current->data para los bytes de la imagen, agregar luego

        current = current->next;
    }
}

struct ImageNode* merge(struct ImageNode* left, struct ImageNode* right) {
    struct ImageNode dummy;
    struct ImageNode* tail = &dummy;
    dummy.next = NULL;

    while (left != NULL && right != NULL) {
        if (left->file_size <= right->file_size) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }

    if (left != NULL) {
        tail->next = left;
    } else {
        tail->next = right;
    }

    return dummy.next;
}

void mergeSort(struct ImageNode** headRef) {
    struct ImageNode* head = *headRef;
    struct ImageNode* left;
    struct ImageNode* right;

    if (head == NULL || head->next == NULL) {
        return; // Base case: empty or single-node list
    }

    struct ImageNode* slow = head;
    struct ImageNode* fast = head->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    left = head;
    right = slow->next;
    slow->next = NULL;

    mergeSort(&left);
    mergeSort(&right);

    *headRef = merge(left, right);
}
