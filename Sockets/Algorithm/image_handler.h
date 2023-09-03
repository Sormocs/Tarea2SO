#ifndef LOAD_IMAGE_H
#define LOAD_IMAGE_H

typedef struct {
    unsigned char* data;
    int width;
    int height;
    int channels;
} ImageData;

ImageData * load_image(const char* path);
void Save_Image(ImageData *image);

#endif
