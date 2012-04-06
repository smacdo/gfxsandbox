/*
 * Copyright 2012 Scott MacDonald
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "texture.h"
#include "glutil.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <GL/glew.h>
#include <cstdlib>
#include <string>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

GLuint loadTexture( const std::string& filename )
{
    std::cout << "Loading texture: " << filename << std::endl;

    // Read the tga texture image in
    GLuint id;
    int width, height;

    void * pPixels = read_tga( filename.c_str(), &width, &height );

    // Make sure it loaded correctly
    assert( pPixels != NULL );

    // Generate a new texture id, and then make it the active texture so we can
    // upload texture data to it
    glGenTextures( 1, &id );
    glBindTexture( GL_TEXTURE_2D, id );

    // Apply texture filtering options as well as uv options
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE );

    // Upload contents of the image into the graphics card
    glTexImage2D(
            GL_TEXTURE_2D,      // target
            0,                  // level of detail
            GL_RGB8,            // internal format
            width,
            height,
            0,                  // border
            GL_BGR,             // incoming (external) format
            GL_UNSIGNED_BYTE,   // incoming (external) type
            pPixels             // dat pixel data
    );

    // Make sure it worked!
    errorCheck( "Uploading texture" );
    free( pPixels );

    return id;
}


short le_short( unsigned char *bytes )
{
    return bytes[0] | ( static_cast<char>(bytes[1]) << 8 );
}

void *read_tga(const char *filename, int *width, int *height)
{
    struct tga_header
    {
       char  id_length;
       char  color_map_type;
       char  data_type_code;
       unsigned char  color_map_origin[2];
       unsigned char  color_map_length[2];
       char  color_map_depth;
       unsigned char  x_origin[2];
       unsigned char  y_origin[2];
       unsigned char  width[2];
       unsigned char  height[2];
       char  bits_per_pixel;
       char  image_descriptor;
    } header;
    int i, color_map_size;
    size_t pixels_size;
    FILE *f;
    size_t read;
    void *pixels;

    f = fopen(filename, "rb");

    if (!f) {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
        return NULL;
    }

    read = fread(&header, 1, sizeof(header), f);

    if (read != sizeof(header)) {
        fprintf(stderr, "%s has incomplete tga header\n", filename);
        fclose(f);
        return NULL;
    }
    if (header.data_type_code != 2) {
        fprintf(stderr, "%s is not an uncompressed RGB tga file\n", filename);
        fclose(f);
        return NULL;
    }
    if (header.bits_per_pixel != 24) {
        fprintf(stderr, "%s is not a 24-bit uncompressed RGB tga file\n", filename);
        fclose(f);
        return NULL;
    }

    for (i = 0; i < header.id_length; ++i)
        if (getc(f) == EOF) {
            fprintf(stderr, "%s has incomplete id string\n", filename);
            fclose(f);
            return NULL;
        }

    color_map_size = le_short(header.color_map_length) * (header.color_map_depth/8);
    for (i = 0; i < color_map_size; ++i)
        if (getc(f) == EOF) {
            fprintf(stderr, "%s has incomplete color map\n", filename);
            fclose(f);
            return NULL;
        }

    *width = le_short(header.width); *height = le_short(header.height);
    pixels_size = *width * *height * (header.bits_per_pixel/8);
    pixels = malloc(pixels_size);

    read = fread(pixels, 1, pixels_size, f);

    if ( read != pixels_size )
    {
        fprintf(stderr, "%s has incomplete image\n", filename);
        fclose(f);
        free(pixels);
        return NULL;
    }

    return pixels;
}

