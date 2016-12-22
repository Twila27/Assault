// By Squirrel Eiserloh

#include "Engine/String/StringUtils.hpp"
#include <stdarg.h>


//-----------------------------------------------------------------------------------------------
const int STRINGF_STACK_LOCAL_TEMP_LENGTH = 2048;


//-----------------------------------------------------------------------------------------------
const std::string Stringf( const char* format, ... )
{
	char textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, STRINGF_STACK_LOCAL_TEMP_LENGTH, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	return std::string( textLiteral );
}


//-----------------------------------------------------------------------------------------------
const std::string Stringf( const int maxLength, const char* format, ... )
{
	char textLiteralSmall[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	char* textLiteral = textLiteralSmall;
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		textLiteral = new char[ maxLength ];

	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, maxLength, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ maxLength - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	std::string returnValue( textLiteral );
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		delete[] textLiteral;

	return returnValue;
}


//-----------------------------------------------------------------------------------------------
const std::string GetAsLowercase( const std::string& mixedCaseString )
{
	std::string copyToTurnLowercase = mixedCaseString;

	auto charIterEnd = copyToTurnLowercase.end();
	for ( auto charIter = copyToTurnLowercase.begin( ); charIter != charIterEnd; charIter++ )
	{
		int uncapitalizedCharAsInt = tolower( *charIter );
		*charIter = static_cast<char>( uncapitalizedCharAsInt );
	}

	return copyToTurnLowercase;
}


//-----------------------------------------------------------------------------------------------
const std::string GetAsUppercase( const std::string& mixedCaseString )
{
	std::string copyToTurnUppercase = mixedCaseString;

	auto charIterEnd = copyToTurnUppercase.end();
	for ( auto charIter = copyToTurnUppercase.begin(); charIter != charIterEnd; charIter++ )
	{
		int capitalizedCharAsInt = toupper( *charIter );
		*charIter = static_cast<char>( capitalizedCharAsInt );
	}

	return copyToTurnUppercase;
}
