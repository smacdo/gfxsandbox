// https://raw.github.com/jckarter/hello-gl/master/util.c
#include "util.h"
#include <cassert>
#include <GL/glew.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

/**
 * Reads the contents of a file into a string and returns it
 */
std::string loadTextFile( const std::string& filename, bool *pStatus )
{
    std::string contents;
    FILE *pFile = fopen( filename.c_str(), "r" );

    // Were we able to open the file?
    if ( pFile == NULL )
    {
        if ( pStatus != NULL )
        {
            *pStatus = false;
        }
    }
    else
    {
        // Wonderful. Seek to the end of the file so we can see how long it is
        fseek( pFile,  0, SEEK_END );
        size_t length = static_cast<size_t>( ftell( pFile ) );
        fseek( pFile, 0, SEEK_SET );

        // Generate a temporary string buffer and read the contents of the file into
        // that buffer
        char * pBuffer = new char[ length + 1 ];
        size_t readlen = fread( pBuffer, 1, length, pFile );

        assert( length == readlen && "File length does not match initial seek" );

        // Ensure that there is a terminating null
        pBuffer[length] = '\0';

        // Now copy the temporary buffer into the string. Once that's done, make
        // sure we do our job and clean up
        contents = std::string( pBuffer );

        delete( pBuffer );
        fclose( pFile );

        // It worked!
        if ( pStatus != NULL )
        {
            *pStatus = true;
        }
    }

    return contents;
}


