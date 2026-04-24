#ifndef FUNCTION_DECLARATIONS
#define FUNCTION_DECLARATIONS

typedef struct
{
    float** image_data; /* a 2D array of floats */
    int m; /* # pixels in vertical-direction */
    int n; /* # pixels in horizontal-direction */
} 
image;

void allocate_image(image *u, int m, int n);

void deallocate_image(image *u);

void convert_jpeg_to_image(const unsigned char* image_chars, image *u);

#endif