#pragma once


#include <string>
#include <vector>


//-----------------------------------------------------------------------------
bool LoadBinaryFileIntoBuffer( const std::string& filePath, std::vector< unsigned char >& out_buffer );
bool LoadFloatsFromTextFileIntoBuffer( const std::string& filePath, std::vector< float >& out_buffer );
bool SaveBufferToBinaryFile( const std::string& filePath, const std::vector< unsigned char >& buffer );
bool SaveFloatsToTextFile( const std::string& filePath, const std::vector < float > & buffer );