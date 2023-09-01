
#ifndef TAREA2SO_IMAGESLIST_H
#define TAREA2SO_IMAGESLIST_H

struct ImageNode {
    char name[256];
    size_t file_size;
    unsigned char *data;
    struct ImageNode *next;
};

struct ImageNode *createImageNode(const char *name, long file_size, const unsigned char *data);
void addImageNode(struct ImageNode **head, const char *name);
void freeImageList(struct ImageNode *head);
void printImageList(struct ImageNode *head);
struct ImageNode* merge(struct ImageNode* left, struct ImageNode* right);
void mergeSort(struct ImageNode** headRef);

#endif //TAREA2SO_IMAGESLIST_H
