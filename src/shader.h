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
#ifndef SCOTT_GFXSANDBOX_SHADER_H
#define SCOTT_GFXSANDBOX_SHADER_H
#include <string>
#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

struct Shader
{
    Shader()
        : program( 0 ),
          vertexShader( 0 ),
          fragmentShader( 0 )
    {
    }

    Shader( GLuint program, GLuint vshader, GLuint fshader )
        : program( program ),
          vertexShader( vshader ),
          fragmentShader( fshader )
    {
    }

    GLuint program;
    GLuint vertexShader;
    GLuint fragmentShader;
};

GLuint loadShader( GLenum type, const std::string& filename );
Shader loadShaderProgram( const std::string& vertexShader,
                          const std::string& fragmentShader );

#endif
