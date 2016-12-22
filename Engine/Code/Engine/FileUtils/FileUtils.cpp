#include "Engine/FileUtils/FileUtils.hpp"
#include <cstdio>


//--------------------------------------------------------------------------------------------------------------
bool LoadBinaryFileIntoBuffer( const std::string& filePath, std::vector< unsigned char >& out_buffer )
{
	FILE* file;
	errno_t err = fopen_s(&file, filePath.c_str(), "rb");
	if ( err != 0 ) return false;
	else
	{
		//Resize vector buffer size to actual file size.
		fseek( file, 0, SEEK_END ); //Move pointer to 0 from end of file.
		size_t fileSize = ftell( file );
		out_buffer.resize( fileSize );

		rewind( file ); //Back to start for fread.
		fread( &out_buffer[ 0 ], sizeof(unsigned char), fileSize, file );

		fclose( file );
	}
	return true;
}


//--------------------------------------------------------------------------------------------------------------
bool LoadFloatsFromTextFileIntoBuffer( const std::string& filePath, std::vector< float >& out_buffer )
{
	FILE* file;
	errno_t err = fopen_s( &file, filePath.c_str(), "rb" );
	if ( err != 0 ) return false;
	else
	{
		float tmpFloat;
		while ( !feof( file ) )
		{
			fscanf_s( file, "%f\n", &tmpFloat );
			out_buffer.push_back( tmpFloat );
		}

		fclose( file );
	}
	return true;
}


//--------------------------------------------------------------------------------------------------------------
bool SaveBufferToBinaryFile( const std::string& filePath, const std::vector< unsigned char>& buffer )
{
	FILE* file;
	errno_t err = fopen_s( &file, filePath.c_str(), "wb" );
	if ( err != 0 ) return false;
	else
	{
		fwrite( &buffer[ 0 ], 1, buffer.size(), file );

		fclose( file );
	}
	return true;
}


//--------------------------------------------------------------------------------------------------------------
bool SaveFloatsToTextFile( const std::string& filePath, const std::vector < float > & buffer )
{
	FILE* file;
	errno_t err = fopen_s( &file, filePath.c_str(), "wb" );
	if ( err != 0 ) return false;
	else
	{
		for ( int bufferIndex = 0; bufferIndex < (int)buffer.size(); bufferIndex++ )
		{
			fprintf( file, "%f\n", buffer[ bufferIndex ] );
		}

		fclose( file );
	}
	return true;
}
