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
#include "gfxsandbox.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include "util.h"
#include "texture.h"
#include "glutil.h"
#include "shader.h"
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

struct Scene
{
    GLuint vertexBuffer, elementBuffer;
    Shader shader;
    GLuint textures[2];

    struct
    {
        GLint fadeFactor;
        GLint textures[2];
    } uniforms;

    struct
    {
        GLint position;
    } attributes;

    GLfloat fadeFactor;
} GScene;

const size_t SQUARE_VERTEX_COUNT = 8;
const GLfloat SQUARE_VERTEX_BUFFER_DATA[ SQUARE_VERTEX_COUNT ] =
{
    -1.0f, -1.0f,
     1.0f, -1.0f,
    -1.0f,  1.0f,
     1.0f,  1.0f
};

const size_t SQUARE_ELEMENT_COUNT = 4;
const GLushort SQUARE_ELEMENT_BUFFER_DATA[SQUARE_ELEMENT_COUNT] =
{
    0, 1, 2, 3
};

int main( int argc, char** argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutInitWindowSize( 640, 480 );
    glutCreateWindow( "Render Window" );
    glutDisplayFunc( &render );
    glutIdleFunc( &update );

    glewInit();

    if (! GLEW_VERSION_2_0 )
    {
        std::cerr << "OpenGL 2.0 not available" << std::endl;
        return EXIT_FAILURE;
    }

    if (! loadResources() )
    {
        std::cerr << "Failed to load resources" << std::endl;
        return EXIT_FAILURE;
    }

    glutMainLoop();
    return EXIT_SUCCESS;
}

bool loadResources()
{
    GScene.shader =
        loadShaderProgram( "content/shaders/hello.vs.glsl",
                           "content/shaders/hello.ps.glsl" );

    GScene.uniforms.fadeFactor =
        glGetUniformLocation( GScene.shader.program, "fade_factor" );
    GScene.uniforms.textures[0] =
        glGetUniformLocation( GScene.shader.program, "textures[0]" );
    GScene.uniforms.textures[1] =
        glGetUniformLocation( GScene.shader.program, "textures[1]" );
    GScene.attributes.position =
        glGetAttribLocation( GScene.shader.program, "position" );

    GScene.vertexBuffer =
        createBufferT<GLfloat>( GL_ARRAY_BUFFER,
                                SQUARE_VERTEX_BUFFER_DATA,
                                sizeof( SQUARE_VERTEX_BUFFER_DATA ) );

    GScene.elementBuffer =
        createBufferT<GLushort> ( GL_ELEMENT_ARRAY_BUFFER,
                                  SQUARE_ELEMENT_BUFFER_DATA,
                                  sizeof(SQUARE_ELEMENT_BUFFER_DATA) );

    GScene.textures[0] = loadTexture( "content/images/hello1.tga" );
    GScene.textures[1] = loadTexture( "content/images/hello2.tga" );

    GScene.fadeFactor  = 0.75f;

    errorCheck( "after loading resources" );

    return true;
}

/**
 * Called whenever the game loop has nothing to do
 */
void update()
{
    int msecs = glutGet( GLUT_ELAPSED_TIME );   // in milliseconds
    GScene.fadeFactor = sinf( (float) msecs * 0.001f ) * 0.5f + 0.5f;
    glutPostRedisplay();
}

/**
 * Called by the game loop to render the current frame
 */
void render()
{
    errorCheck( "About to render" );

    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    glUseProgram( GScene.shader.program );
    errorCheck( "Using shader in render" );

    glUniform1f( GScene.uniforms.fadeFactor, GScene.fadeFactor );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, GScene.textures[0] );
    glUniform1i( GScene.uniforms.textures[0], 0 );

    glActiveTexture( GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_2D, GScene.textures[1] );
    glUniform1i( GScene.uniforms.textures[1], 1 );

    errorCheck( "Assign attributes and uniforms in render" );

    glBindBuffer( GL_ARRAY_BUFFER, GScene.vertexBuffer );
    glVertexAttribPointer(
            GScene.attributes.position,
            2,                      // two elements (x,y)
            GL_FLOAT,               // of type float
            GL_FALSE,               // normalized? (values mapped into range)
            sizeof(GLfloat) * 2,    // vertex stride
            (void*) 0               // array buffer offset, pointer type is historic
    );
    glEnableVertexAttribArray( GScene.attributes.position );
    errorCheck( "Assigning vertex buffer attribute" );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, GScene.elementBuffer );
    glDrawElements( GL_TRIANGLE_STRIP,  // mode
                    4,                  // num vertices
                    GL_UNSIGNED_SHORT,  // data type
                    (void*) 0           // array buffer offset
    );

    errorCheck( "Binding the element buffer" );
    glDisableVertexAttribArray( GScene.attributes.position );

    glutSwapBuffers();
    errorCheck( "after render" );
}





