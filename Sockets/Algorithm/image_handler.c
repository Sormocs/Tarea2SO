#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#include "image_handler.h"

ImageData * load_image(const char *path){
    int width, height, channels;
    unsigned char *imageData = stbi_load(path, &width, &height, &channels, STBI_rgb);

    ImageData *image;
    image->data = imageData;
    image->width = width;
    image->height = height;
    image->channels = channels;

    if (!image->data) {
        fprintf(stderr, "Failed to load the image: %s\n", path);
        image->width = image->height = image->channels = 0;
    } else {
        printf("Image width: %d\n", image->width);
        printf("Image height: %d\n", image->height);
    }

    return image;
}

void Save_Image(ImageData *image){

    int r,g,b;
    for(int i = 0; i < image->width*image->height * 3; i+=3){
        int red, green, blue;
        red = image->data[i];
        green = image->data[i+1];
        blue = image->data[i+2];
        if (red > green && red > blue){
            r++;
        } else if (green > red && green > blue){
            g++;
        } else if (blue > red && blue > green){
            b++;
        }
    }

    if (r > g && r > b){
        stbi_write_png("./Output/Red/output_equ.png", image->width, image->height, image->channels, image->data, image->width * image->channels);
    } else if (g > r && g > b){
        stbi_write_png("./Output/Green/output_equ.png", image->width, image->height, image->channels, image->data, image->width * image->channels);
    } else if (b > r && b > g){
        stbi_write_png("./Output/Blue/output_equ.png", image->width, image->height, image->channels, image->data, image->width * image->channels);
    } else {
        stbi_write_png("./Output/Undefined/output_equ.png", image->width, image->height, image->channels, image->data, image->width * image->channels);
    }
}