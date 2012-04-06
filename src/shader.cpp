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
#include "shader.h"
#include "glutil.h"
#include "util.h"
#include <iostream>
#include <cassert>
#include <memory>
#include <vector>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/**
 * Creates a new shader object by loading the requested vertex shader and
 * fragment shader. The engine will load both of these from disk, compile them
 * and then assemble the final shader program.
 *
 * \param  vertexShader    Path to the vertex shader
 * \param  fragmentShader  Path to the fragment shader
 * \return                 Shader object containing details on the shader
 */
Shader loadShaderProgram( const std::string& vertexShader,
                          const std::string& fragmentShader )
{
    Shader shader;

    // First attempt to load and compile the requested vertex and fragment
    // shaders
    shader.vertexShader   = loadShader( GL_VERTEX_SHADER, vertexShader );
    shader.fragmentShader = loadShader( GL_FRAGMENT_SHADER, fragmentShader ); 

    // Generate a new shader program
    shader.program = glCreateProgram();

    // Attach the vertex and fragment shaders
    glAttachShader( shader.program, shader.vertexShader );
    glAttachShader( shader.program, shader.fragmentShader );
    glLinkProgram( shader.program );

    errorCheck( "Linking shader" );

    // Check if we were able to succesfully create the shader program
    GLint ok = 0;
    glGetProgramiv( shader.program, GL_LINK_STATUS, &ok );

    if (! ok )
    {
        std::string message =
            showInfoLog( shader.program, glGetProgramiv, glGetProgramInfoLog );

        std::cerr << "Failed to link shader program" << std::endl;
        std::cerr << "ERROR: " << message << std::endl;

        exit( 1 );
    }

    return shader;
}

/**
 * Loads a GLSL shader from disk, compiles it and returns it's assigned object
 * id
 *
 * \param  type      The type of OpenGL shader to create
 * \param  filename  Path to the shader code
 * \return           Shader's object id
 */
GLuint loadShader( GLenum type, const std::string& filename )
{
    std::cout << "Loading shader: " << filename << std::endl;

    // Load the shader source code from disk and verify that everything went
    // according to plan
    bool didWork = false;
    std::string source = loadTextFile( filename, &didWork );

    if (! didWork )
    {
        std::cerr << "Failed to load shader from disk: " << filename << std::endl;
        exit( 1 );
    }
    
    // Generate a new shader object
    GLuint shader = glCreateShader( type );

    // Load the source code into the hardware, and then instruct OpenGL to
    // compile it into machine bytecode.
    //
    // OpennGL really wants the shader to be an array of char*... not sure why
    const char * pSource = source.c_str();
    int sourceLength     = static_cast<int>( source.size() );

    glShaderSource( shader, 1, &pSource, &sourceLength );
    glCompileShader( shader );

    // Check to see if the shader successfully compiled. If it didn't, go figure
    // out why
    GLint ok = 0;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &ok );

    if (! ok )
    {
        std::cerr << "Failed to compile: " << filename << std::endl;
        std::string error = showInfoLog( shader, glGetShaderiv, glGetShaderInfoLog );
        std::cerr << "ERROR: " << error << std::endl;

        exit( 1 );
    }

    return shader;
}

