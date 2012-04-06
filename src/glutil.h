#ifndef SCOTT_GFXSANDBOX_GLUTIL_H
#define SCOTT_GFXSANDBOX_GLUTIL_H

#include <GL/glew.h>
#include <string>
#include <vector>

// Create a hardware buffer with the given array of untyped data
GLuint createBuffer( GLenum target,
                     const void* pData,
                     GLsizei bufferSize,
                     bool * ok = NULL );

// Create a hardware buffer with the given array of typed data
template<typename T>
GLuint createBufferT( GLenum target, const std::vector<T>& array );

// Create a hardware buffer with the given array of typed data
template<typename T>
GLuint createBufferT( GLenum target, const T* pElements, size_t count );

// Check if there were any OpenGL errors
bool errorCheck( const char* state = NULL, bool dieOnError = true );

// Look up log info
std::string showInfoLog( GLuint object,
                         PFNGLGETSHADERIVPROC glGet__iv,
                         PFNGLGETSHADERINFOLOGPROC glGet__InfoLog );

/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
template<typename T>
GLuint createBufferT( GLenum target, const std::vector<T>& array )
{
    return createBuffer( target, &array[0], array.size() * sizeof(T) );
}

template<typename T>
GLuint createBufferT( GLenum target, const T* pElements, size_t count )
{
    return createBuffer( target, pElements, count * sizeof(T) );
}

#endif
