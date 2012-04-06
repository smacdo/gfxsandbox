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
#include "glutil.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <sstream>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/**
 * Generates a OpenGL data buffer and fills it with the requested data
 *
 * \param  target  Type of OpenGL buffer to create
 * \param  pData   Pointer to a data byte buffer
 * \return         The id of the generated buffer object
 */
GLuint createBuffer( GLenum target,
                     const void* pData,
                     GLsizei bufferSize,
                     bool * pOk )
{
    assert( pData != NULL && "Cannot upload if data pointer is null" );
    assert( bufferSize > 0 && "Cannot upload if there is no data to upload" );

    GLuint id = 0;

    // Request a new hardware buffer id, bind to it and then upload all of the
    // data we were given
    glGenBuffers( 1, &id );
    glBindBuffer( target, id );
    glBufferData( target, bufferSize, pData, GL_STATIC_DRAW );

    // Verify that the buffer creation succeeded
    bool ok = errorCheck( "Creating data buffer", false );

    if ( pOk != NULL )
    {
        *pOk = ok;
    }
    
    // Return id of the newly generated buffer
    return id;
}


/**
 * Queries OpenGL for information on a particular function call, and returns a
 * string containging that information
 */
std::string showInfoLog( GLuint object,
                         PFNGLGETSHADERIVPROC glGet__iv,
                         PFNGLGETSHADERINFOLOGPROC glGet__InfoLog )
{
    GLint length = 0;

    // Get the length of the info object
    glGet__iv( object, GL_INFO_LOG_LENGTH, &length );

    // Generate a temporary char buffer and pass it to OpenGL to fill up with
    // our log details
    std::vector<GLchar> buffer( length );
    glGet__InfoLog( object, length, NULL, &buffer[0] );

    // Convert the chra array into a string and return it to the caller
    return std::string( &buffer[0] );
}

/**
 * Checks if there are any active OpenGL errors. This method will print out 
 * details of any error conditions that exist. Additionally, this method can
 * automatically terminate the running program upon detection of an error
 *
 * \param  state       Description of the program's current state
 * \param  dieOnError  True to automatically exit if there's an error
 * \return             True if there was an error, false otherwise
 */
bool errorCheck( const char* state, bool dieOnError )
{
    GLenum error   = glGetError();
    bool hasErrors = true;
    
    if ( error == GL_NO_ERROR )
    {
        // We're clear!
        hasErrors = false;
    }
    else
    {
        std::ostringstream ss;

        // Print out an error header first
        ss << "OpenGL error occurred";

        if ( state != NULL )
        {
            ss << " in " << state << ": ";
        }
        else
        {
            ss << ": ";
        }

        switch ( error )
        {
            case GL_INVALID_ENUM:
                ss << "GLenum argument out of range";
                break;

            case GL_INVALID_VALUE:
                ss << "Numeric argument out of range";
                break;

            case GL_INVALID_OPERATION:
                ss << "Operation illegal in current state";
                break;

            case GL_STACK_OVERFLOW:
                ss << "Command would cause stack overflow";
                break;

            case GL_STACK_UNDERFLOW:
                ss << "Command would cause stack underflow";
                break;

            case GL_OUT_OF_MEMORY:
                ss << "Not enough memory left to execute command";
                break;

            default:
                ss << "Unknown error code returned '" << error << "'";
                break;
        }

        std::cerr << ss.str() << std::endl;

        // Should we exit on an error?
        if ( dieOnError )
        {
            exit( 1 );
        }
    }

    return hasErrors;
}
