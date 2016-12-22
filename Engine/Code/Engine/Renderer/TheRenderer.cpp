#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/EngineCommon.hpp"
#include "Engine/Renderer/OpenGLExtensions.hpp"


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library
#pragma comment( lib, "GLu32" ) 

#include "Engine/Renderer/OpenGLExtensions.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/AABB3.hpp"


//--------------------------------------------------------------------------------------------------------------
TheRenderer* g_theRenderer = nullptr;


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::CreateBuiltInTextures()
{
	unsigned char plainWhiteTexel[ 3 ] = { 255, 255, 255 };
	TheRenderer::m_defaultTexture = Texture::CreateTextureFromBytes( "PlainWhite", plainWhiteTexel, IntVector2::ONE, 3 );
}


//--------------------------------------------------------------------------------------------------------------
unsigned int TheRenderer::GetOpenGLVertexGroupingRule(unsigned int TheRendererVertexGroupingRule) const
{
	switch ( TheRendererVertexGroupingRule )
	{
		case VertexGroupingRule::AS_LINES: return GL_LINES;
		case VertexGroupingRule::AS_POINTS: return GL_POINTS;
		case VertexGroupingRule::AS_LINE_LOOP: return GL_LINE_LOOP;
		case VertexGroupingRule::AS_LINE_STRIP: return GL_LINE_STRIP;
		case VertexGroupingRule::AS_QUADS: return GL_QUADS;
		default: return GL_POINTS;
	}
}


//--------------------------------------------------------------------------------------------------------------
TheRenderer::TheRenderer()
	:m_defaultFont( BitmapFont::CreateOrGetFont( "Data/Fonts/SquirrelFixedFont.png" ) )
{
	DebuggerPrintf( "OpenGL Vendor is: %s\n", glGetString( GL_VENDOR ) );
	DebuggerPrintf( "OpenGL Version is: %s\n", glGetString( GL_VERSION ) );

	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress( "glGenBuffers" );
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress( "glBindBuffer" );
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress( "glBufferData" );
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress( "glDeleteBuffers" );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_LINE_SMOOTH );
	glLineWidth( 1.5f );

	CreateBuiltInTextures();
}


//--------------------------------------------------------------------------------------------------------------
TheRenderer::~TheRenderer()
{
	delete m_defaultFont;
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::ClearScreenToColor( const Rgba& colorToClearTo )
{
	//Only seems to take floats, not uchar values. Better to pack with uchar, but costs to range map back.
	float red = Interpolate( 0.f, 1.f, ( static_cast<float>( colorToClearTo.red ) / 255.f ) );
	float green = Interpolate( 0.f, 1.f, ( static_cast<float>( colorToClearTo.red ) / 255.f ) );
	float blue = Interpolate( 0.f, 1.f, ( static_cast<float>( colorToClearTo.red ) / 255.f ) );

	glClearColor( red, green, blue, 1.f );

	glClear( GL_COLOR_BUFFER_BIT );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::ClearScreenToColor( float red, float green, float blue )
{
	glClearColor( red, green, blue, 1.f );

	glClear( GL_COLOR_BUFFER_BIT );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::ClearScreenDepthBuffer()
{
	glClearDepth( 1.f ); //Says push all pixels to maximum far-away so that any pixel will be closer now, this is normalized 0-1.
	glClear( GL_DEPTH_BUFFER_BIT );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::EnableDepthTesting( bool flagValue )
{
	if ( flagValue ) glEnable( GL_DEPTH_TEST );
	else glDisable( GL_DEPTH_TEST );
}

//--------------------------------------------------------------------------------------------------------------
void TheRenderer::EnableAlphaTesting( bool flagValue )
{
	if ( flagValue ) glEnable( GL_ALPHA_TEST );
	else glDisable( GL_ALPHA_TEST );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::SetAlphaFunc( int alphaComparatorFunction, float alphaComparatorValue )
{
	glAlphaFunc( alphaComparatorFunction, alphaComparatorValue );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::EnableBackfaceCulling( bool flagValue )
{
	if ( flagValue ) glEnable( GL_CULL_FACE );
	else glDisable( GL_CULL_FACE );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::CreateVbo( unsigned int& out_vboID )
{
	glGenBuffers( 1, &out_vboID );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::UpdateVbo( unsigned int vboID, const Vertex3D_PCT* vertexArrayData, unsigned int vertexArraySizeInBytes )
{
	glBindBuffer( GL_ARRAY_BUFFER, vboID );

	glBufferData( GL_ARRAY_BUFFER, vertexArraySizeInBytes, vertexArrayData, GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::BindVbo( unsigned int vboID )
{
	glBindBuffer( GL_ARRAY_BUFFER, vboID );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DestroyVbo( unsigned int vboID )
{
	glDeleteBuffers( 1, &vboID );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DrawVbo_PCT( unsigned int vboID, int numVerts, VertexGroupingRule vertexGroupingRule )
{
	if ( numVerts == 0 ) return;

	glBindBuffer( GL_ARRAY_BUFFER, vboID );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer( 3, GL_FLOAT, sizeof( Vertex3D_PCT ), (const GLvoid*)offsetof( Vertex3D_PCT, m_position ) );
	glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof( Vertex3D_PCT ), (const GLvoid*)offsetof( Vertex3D_PCT, m_color ) );
	glTexCoordPointer( 2, GL_FLOAT, sizeof( Vertex3D_PCT ), (const GLvoid*)offsetof( Vertex3D_PCT, m_texCoords ) );

	glDrawArrays( GetOpenGLVertexGroupingRule( vertexGroupingRule ), 0, numVerts );

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	UnbindTexture();
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::SetDrawColor( float red, float green, float blue, float opacity )
{
	glColor4f( red, green, blue, opacity );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::SetLineWidth( float newLineWidth )
{
	glLineWidth( newLineWidth );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::SetBlendFunc( int sourceBlend, int destinationBlend )
{
	glBlendFunc( sourceBlend, destinationBlend );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::SetRenderFlag( int flagNameToSet )
{
	glEnable( flagNameToSet );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::SetPointSize( float thickness )
{
	glPointSize( thickness );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::SetOrtho( const Vector2& bottomLeft, const Vector2& topRight )
{
	glLoadIdentity( ); //Ensures we clear it all so we have a fresh slate to set things on.

	glOrtho( bottomLeft.x, topRight.x, bottomLeft.y, topRight.y, 0.f, 1.f );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::SetPerspective( float fovDegreesY, float aspect, float nearDist, float farDist )
{
	glLoadIdentity();

	gluPerspective( fovDegreesY, aspect, nearDist, farDist );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::TranslateView( const Vector2& translation )
{
	glTranslatef( translation.x, translation.y, 0.f );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::TranslateView( const Vector3& translation )
{
	glTranslatef( translation.x, translation.y, translation.z );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::RotateViewByDegrees( float degrees, const Vector3& axisOfRotation /*= Vector3( 0.f, 0.f, 1.f )*/ )
{
	glRotatef( degrees, axisOfRotation.x, axisOfRotation.y, axisOfRotation.z );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::RotateViewByRadians( float radians, const Vector3& axisOfRotation /*= Vector3( 0.f, 0.f, 1.f )*/ )
{
	glRotatef( ConvertRadiansToDegrees( radians ), axisOfRotation.x, axisOfRotation.y, axisOfRotation.z );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::ScaleView( float uniformScale )
{
	glScalef( uniformScale, uniformScale, 1.f );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::PushView()
{
	glPushMatrix( );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::PopView()
{
	glPopMatrix( );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DrawPoint( const Vector3& position, float thickness, const Rgba& color /*= Rgba() */ )
{
	glPointSize( thickness );

	UnbindTexture();

	DrawVertexArray_PCT( AS_POINTS, { Vertex3D_PCT( position, color ) }, 1 );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DrawLine( const Vector2& startPos, const Vector2& endPos, 
	const Rgba& startColor /*=Rgba()*/, const Rgba& endColor /*=Rgba()*/, float lineThickness /*= 1.f */ )
{
	glLineWidth( lineThickness );

	UnbindTexture();

	Vector3 startPos3D = Vector3( startPos.x, startPos.y, 0.f );
	Vector3 endPos3D = Vector3( endPos.x, endPos.y, 0.f );
	DrawVertexArray_PCT( AS_LINES, { Vertex3D_PCT( startPos3D, startColor ), Vertex3D_PCT( endPos3D, endColor ) }, 2 );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DrawLine( const Vector3& startPos, const Vector3& endPos, const Rgba& startColor /*= Rgba()*/, const Rgba& endColor /*= Rgba()*/, float lineThickness /*= 1.f */ )
{
	glLineWidth( lineThickness );

	UnbindTexture();

	DrawVertexArray_PCT( AS_LINES, { Vertex3D_PCT( startPos, startColor ), Vertex3D_PCT( endPos, endColor ) }, 2 );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DrawAABB( const int vertexGroupingRule, const AABB3& bounds, const Texture& texture, const AABB2* texCoords, const Rgba& tint /*= Rgba()*/, float lineThickness /*= 1.f*/ )
{
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, texture.GetTextureID() );

	glColor4ub( tint.red, tint.green, tint.blue, tint.alphaOpacity );

	glLineWidth( lineThickness );

	Vertex3D_PCT vertexes[24];

	//Bottom.
	vertexes[ 0 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, bounds.mins.z ), Vector2( texCoords[ 0 ].mins.x, texCoords[ 0 ].maxs.y ), tint );
	vertexes[ 1 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, bounds.mins.z ), Vector2( texCoords[ 0 ].maxs.x, texCoords[ 0 ].maxs.y ), tint );
	vertexes[ 2 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, bounds.mins.z ), Vector2( texCoords[ 0 ].maxs.x, texCoords[ 0 ].mins.y ), tint );
	vertexes[ 3 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, bounds.mins.z ), Vector2( texCoords[ 0 ].mins.x, texCoords[ 0 ].mins.y ), tint );

	//Top.
	vertexes[ 4 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, bounds.maxs.z ), Vector2( texCoords[ 1 ].mins.x, texCoords[ 1 ].maxs.y ), tint );
	vertexes[ 5 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, bounds.maxs.z ), Vector2( texCoords[ 1 ].maxs.x, texCoords[ 1 ].maxs.y ), tint );
	vertexes[ 6 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, bounds.maxs.z ), Vector2( texCoords[ 1 ].maxs.x, texCoords[ 1 ].mins.y ), tint );
	vertexes[ 7 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, bounds.maxs.z ), Vector2( texCoords[ 1 ].mins.x, texCoords[ 1 ].mins.y ), tint );

	//Left.
	vertexes[ 8  ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, bounds.mins.z ), Vector2( texCoords[ 2 ].mins.x, texCoords[ 2 ].maxs.y ), tint );
	vertexes[ 9  ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, bounds.mins.z ), Vector2( texCoords[ 2 ].maxs.x, texCoords[ 2 ].maxs.y ), tint );
	vertexes[ 10 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, bounds.maxs.z ), Vector2( texCoords[ 2 ].maxs.x, texCoords[ 2 ].mins.y ), tint );
	vertexes[ 11 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, bounds.maxs.z ), Vector2( texCoords[ 2 ].mins.x, texCoords[ 2 ].mins.y ), tint );

	//Right.
	vertexes[ 12 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, bounds.mins.z ), Vector2( texCoords[ 3 ].mins.x, texCoords[ 3 ].maxs.y ), tint );
	vertexes[ 13 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, bounds.mins.z ), Vector2( texCoords[ 3 ].maxs.x, texCoords[ 3 ].maxs.y ), tint );
	vertexes[ 14 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, bounds.maxs.z ), Vector2( texCoords[ 3 ].maxs.x, texCoords[ 3 ].mins.y ), tint );
	vertexes[ 15 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, bounds.maxs.z ), Vector2( texCoords[ 3 ].mins.x, texCoords[ 3 ].mins.y ), tint );

	//Front.
	vertexes[ 16 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, bounds.mins.z ), Vector2( texCoords[ 4 ].mins.x, texCoords[ 4 ].maxs.y ), tint );
	vertexes[ 17 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, bounds.mins.z ), Vector2( texCoords[ 4 ].maxs.x, texCoords[ 4 ].maxs.y ), tint );
	vertexes[ 18 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, bounds.maxs.z ), Vector2( texCoords[ 4 ].maxs.x, texCoords[ 4 ].mins.y ), tint );
	vertexes[ 19 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, bounds.maxs.z ), Vector2( texCoords[ 4 ].mins.x, texCoords[ 4 ].mins.y ), tint );

	//Back.
	vertexes[ 20 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, bounds.mins.z ), Vector2( texCoords[ 5 ].mins.x, texCoords[ 5 ].maxs.y ), tint );
	vertexes[ 21 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, bounds.mins.z ), Vector2( texCoords[ 5 ].maxs.x, texCoords[ 5 ].maxs.y ), tint );
	vertexes[ 22 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, bounds.maxs.z ), Vector2( texCoords[ 5 ].maxs.x, texCoords[ 5 ].mins.y ), tint );
	vertexes[ 23 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, bounds.maxs.z ), Vector2( texCoords[ 5 ].mins.x, texCoords[ 5 ].mins.y ), tint );

	DrawVertexArray_PCT( vertexGroupingRule, vertexes, 24 );

	UnbindTexture();
}


//----------------------//IMPORTANT: vertices iterated CCW from bottom-left, but texels from TOP-left in OpenGL.
void TheRenderer::DrawAABB( const int vertexGroupingRule, const AABB2& bounds, const Texture& texture,
	const AABB2& texCoords /*= AABB2(0,0,1,1)*/, const Rgba& tint  /*=Rgba()*/, float lineThickness /*= 1.f */ )
{
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, texture.GetTextureID() );
	
	glLineWidth( lineThickness );

	Vertex3D_PCT vertexes[4];

	vertexes[ 0 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, 0.f ), Vector2( texCoords.mins.x, texCoords.maxs.y ), tint );
	vertexes[ 1 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, 0.f ), Vector2( texCoords.maxs.x, texCoords.maxs.y ), tint );
	vertexes[ 2 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, 0.f ), Vector2( texCoords.maxs.x, texCoords.mins.y ), tint );
	vertexes[ 3 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, 0.f ), Vector2( texCoords.mins.x, texCoords.mins.y ), tint );

	DrawVertexArray_PCT( vertexGroupingRule, vertexes, 4 );
	
	UnbindTexture();
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DrawAABB( const int vertexGroupingRule, const AABB2& bounds, const Rgba& color /*=Rgba()*/, float lineThickness /*= 1.f */ )
{
	UnbindTexture();
	
	glLineWidth( lineThickness );
	
	Vertex3D_PCT vertexes[ 4 ];

	vertexes[ 0 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, 0.f ), color );
	vertexes[ 1 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, 0.f ), color );
	vertexes[ 2 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, 0.f ), color );
	vertexes[ 3 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, 0.f ), color );

	DrawVertexArray_PCT( vertexGroupingRule, vertexes, 4 );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DrawAABB( const int vertexGroupingRule, const AABB3& bounds, const Rgba& color /*= Rgba()*/, float lineThickness /*= 1.f */ )
{
	UnbindTexture();

	glLineWidth( lineThickness );


	Vertex3D_PCT vertexes[24];

	//Bottom.
	vertexes[ 0 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, bounds.mins.z ), color );
	vertexes[ 1 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, bounds.mins.z ), color );
	vertexes[ 2 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, bounds.mins.z ), color );
	vertexes[ 3 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, bounds.mins.z ), color );
			
	//Top.
	vertexes[ 4 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, bounds.maxs.z ), color );
	vertexes[ 5 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, bounds.maxs.z ), color );
	vertexes[ 6 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, bounds.maxs.z ), color );
	vertexes[ 7 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, bounds.maxs.z ), color );
			
	//Left.
	vertexes[ 8 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, bounds.mins.z ), color );
	vertexes[ 9 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, bounds.mins.z ), color );
	vertexes[ 10 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, bounds.maxs.z ), color );
	vertexes[ 11 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, bounds.maxs.z ), color );
			
	//Right.
	vertexes[ 12 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, bounds.maxs.z ), color );
	vertexes[ 13 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, bounds.mins.z ), color );
	vertexes[ 14 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, bounds.mins.z ), color );
	vertexes[ 15 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, bounds.maxs.z ), color );
			
	//Front.
	vertexes[ 16 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, bounds.maxs.z ), color );
	vertexes[ 17 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, bounds.maxs.z ), color );
	vertexes[ 18 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, bounds.mins.z ), color );
	vertexes[ 19 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, bounds.mins.z ), color );
			
	//Back.	
	vertexes[ 20 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, bounds.mins.z ), color );
	vertexes[ 21 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, bounds.mins.z ), color );
	vertexes[ 22 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, bounds.maxs.z ), color );
	vertexes[ 23 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, bounds.maxs.z ), color );

	DrawVertexArray_PCT( vertexGroupingRule, vertexes, 24 ); //Be wary of grouping rule affliction by above ordering, e.g. line loop.
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DrawShadedAABB( const int vertexGroupingRule, const AABB2& bounds, const Rgba& topLeftColor /*=Rgba()*/, const Rgba& topRightColor /*=Rgba()*/,
	const Rgba& bottomLeftColor /*=Rgba()*/, const Rgba& bottomRightColor /*=Rgba()*/, float lineThickness /*= 1.f */ )
{
	UnbindTexture();

	glLineWidth( lineThickness );

	Vertex3D_PCT vertexes[ 4 ];

	vertexes[ 0 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, 0.f ), bottomLeftColor );
	vertexes[ 1 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, 0.f ), bottomRightColor );
	vertexes[ 2 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, 0.f ), topRightColor );
	vertexes[ 3 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, 0.f ), topLeftColor );

	DrawVertexArray_PCT( vertexGroupingRule, vertexes, 4 );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DrawShadedAABB( const int vertexGroupingRule, const AABB3& bounds, const Rgba& topLeftColor /*= Rgba()*/, const Rgba& topRightColor /*= Rgba()*/, const Rgba& bottomLeftColor /*= Rgba()*/, const Rgba& bottomRightColor /*= Rgba()*/, float lineThickness /*= 1.f */ )
{
	UnbindTexture();

	glLineWidth( lineThickness );


	Vertex3D_PCT vertexes[ 24 ];

	//Bottom.
	vertexes[ 0 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, bounds.mins.z ), topRightColor );
	vertexes[ 1 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, bounds.mins.z ), bottomRightColor );
	vertexes[ 2 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, bounds.mins.z ), bottomLeftColor );
	vertexes[ 3 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, bounds.mins.z ), topLeftColor );

	//Top.
	vertexes[ 4 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, bounds.maxs.z ), topRightColor );
	vertexes[ 5 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, bounds.maxs.z ), bottomRightColor );
	vertexes[ 6 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, bounds.maxs.z ), bottomLeftColor );
	vertexes[ 7 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, bounds.maxs.z ), topLeftColor );

	//Left.
	vertexes[ 8 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, bounds.mins.z ), topRightColor );
	vertexes[ 9 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, bounds.mins.z ), bottomRightColor );
	vertexes[ 10 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, bounds.maxs.z ), bottomLeftColor );
	vertexes[ 11 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, bounds.maxs.z ), topLeftColor );

	//Right.
	vertexes[ 12 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, bounds.maxs.z ), topRightColor );
	vertexes[ 13 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, bounds.mins.z ), bottomRightColor );
	vertexes[ 14 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, bounds.mins.z ), bottomLeftColor );
	vertexes[ 15 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, bounds.maxs.z ), topLeftColor );

	//Front.
	vertexes[ 16 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, bounds.maxs.z ), topRightColor );
	vertexes[ 17 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, bounds.maxs.z ), bottomRightColor );
	vertexes[ 18 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.maxs.y, bounds.mins.z ), bottomLeftColor );
	vertexes[ 19 ] = Vertex3D_PCT( Vector3( bounds.mins.x, bounds.mins.y, bounds.mins.z ), topLeftColor );

	//Back.	
	vertexes[ 20 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, bounds.mins.z ), topRightColor );
	vertexes[ 21 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, bounds.mins.z ), bottomRightColor );
	vertexes[ 22 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.maxs.y, bounds.maxs.z ), bottomLeftColor );
	vertexes[ 23 ] = Vertex3D_PCT( Vector3( bounds.maxs.x, bounds.mins.y, bounds.maxs.z ), topLeftColor );

	DrawVertexArray_PCT( vertexGroupingRule, vertexes, 24 ); //Be wary of grouping rule affliction by above ordering, e.g. line loop.
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DrawQuad( const int vertexGroupingRule, const Vector2& topLeft, const Vector2& topRight,
	const Vector2& bottomRight, const Vector2& bottomLeft, const Rgba& color /*=Rgba()*/, float lineThickness /*= 1.f */ )
{
	UnbindTexture();

	glLineWidth( lineThickness );

	Vertex3D_PCT vertexes[ 4 ];

	vertexes[ 0 ] = Vertex3D_PCT( Vector3( topLeft.x, topLeft.y, 0.f ), color );
	vertexes[ 1 ] = Vertex3D_PCT( Vector3( topRight.x, topRight.y, 0.f ), color );
	vertexes[ 2 ] = Vertex3D_PCT( Vector3( bottomRight.x, bottomRight.y, 0.f ), color );
	vertexes[ 3 ] = Vertex3D_PCT( Vector3( bottomLeft.x, bottomLeft.y, 0.f ), color );

	DrawVertexArray_PCT( vertexGroupingRule, vertexes, 4 );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DrawPolygon( const int vertexGroupingRule, const Vector2& centerPos, float radius, float numSides, float degreesOffset, const Rgba& color /*=Rgba()*/, float lineThickness /*=1.0f*/ )
{
	std::vector<Vertex3D_PCT> vertexes;

	const float radiansTotal = fTWO_PI;
	const float radiansPerSide = radiansTotal / numSides;

	UnbindTexture();

	glLineWidth( lineThickness );

	for ( float radians = 0.f; radians < radiansTotal; radians += radiansPerSide ) {
		float rotatedRadians = radians + ConvertDegreesToRadians( degreesOffset );
		float x = centerPos.x + ( radius * cos( rotatedRadians ) );
		float y = centerPos.y + ( radius * sin( rotatedRadians ) );
		vertexes.push_back( Vertex3D_PCT( Vector3( x, y, 0.f ), color ) );
	}

	DrawVertexArray_PCT( vertexGroupingRule, vertexes, vertexes.size() );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DrawText2D( const Vector2& startBottomLeft, const std::string& asciiText, float cellHeight, const Rgba& tint /*= Rgba()*/, const BitmapFont* font /*= nullptr*/, float cellAspect /*= 1.f */, bool drawDropShadow /*= true*/ )
{
	if ( font == nullptr ) font = m_defaultFont;

	Vector2 glyphBottomLeft = startBottomLeft;
	Vector2 cellSize( cellAspect * cellHeight, cellHeight );

	for ( int stringIndex = 0; stringIndex < (int)asciiText.size( ); stringIndex++ )
	{
		AABB2 texCoords = font->GetTexCoordsForGlyph( asciiText[ stringIndex ] );

		Vector2 glyphTopRight = glyphBottomLeft + cellSize;
		AABB2 renderBounds = AABB2( glyphBottomLeft, glyphTopRight );
		Texture* fontTexture = font->GetFontTexture();

		if ( drawDropShadow )
		{
			AABB2 shadowRenderBounds = renderBounds;
			Vector2 shadowOffset = Vector2( 2.f, -2.f );
			shadowRenderBounds.mins += shadowOffset;
			shadowRenderBounds.maxs += shadowOffset;
			g_theRenderer->DrawAABB( VertexGroupingRule::AS_QUADS, shadowRenderBounds, *fontTexture, texCoords, Rgba::BLACK );
		}
		g_theRenderer->DrawAABB( VertexGroupingRule::AS_QUADS, renderBounds, *fontTexture, texCoords, tint );

		glyphBottomLeft.x += cellSize.x;
	}
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DrawAxes( float length, float lineThickness /*=1.f*/, float alphaOpacity /*=1.f*/, bool drawZ /*=false*/ )
{
	UnbindTexture();

	glLineWidth( lineThickness );

	DrawLine( Vector3( 0.f, 0.f, 0.f ), Vector3( length, 0.f, 0.f ), Rgba( 1.f, 0.f, 0.f, alphaOpacity ), Rgba( 1.f, 0.f, 0.f, alphaOpacity ), lineThickness );
	DrawLine( Vector3( 0.f, 0.f, 0.f ), Vector3( 0.f, length, 0.f ), Rgba( 0.f, 1.f, 0.f, alphaOpacity ), Rgba( 0.f, 1.f, 0.f, alphaOpacity ), lineThickness );
	if ( drawZ ) DrawLine( Vector3( 0.f, 0.f, 0.f ), Vector3( 0.f, 0.f, length ), Rgba( 0.f, 0.f, 1.f, alphaOpacity ), Rgba( 0.f, 0.f, 1.f, alphaOpacity ), lineThickness );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DrawDebugAxes( float length /*= 1.f*/, float lineThickness /*=1.f*/, bool drawZ /*=false*/ )
{
	g_theRenderer->EnableDepthTesting( true );
	g_theRenderer->DrawAxes( length, lineThickness, 1.f, drawZ );

	g_theRenderer->EnableDepthTesting( false );
	g_theRenderer->DrawAxes( length, 3.f * lineThickness, .3f, drawZ );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DrawVertexArray_PCT( const int vertexGroupingRule, const std::vector< Vertex3D_PCT >& vertexArrayData, unsigned int vertexArraySize )
{
	if ( vertexArraySize == 0 ) return;

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer( 3, GL_FLOAT, sizeof( Vertex3D_PCT ), &vertexArrayData[ 0 ].m_position );
	glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof ( Vertex3D_PCT ), &vertexArrayData[ 0 ].m_color );
	glTexCoordPointer( 2, GL_FLOAT, sizeof( Vertex3D_PCT ), &vertexArrayData[ 0 ].m_texCoords );

	glDrawArrays( GetOpenGLVertexGroupingRule( vertexGroupingRule ), 0, vertexArraySize );

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	UnbindTexture();
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DrawVertexArray_PCT( const int vertexGroupingRule, const Vertex3D_PCT* vertexArrayData, unsigned int vertexArraySize )
{
	if ( vertexArraySize == 0 ) return;

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer( 3, GL_FLOAT, sizeof( Vertex3D_PCT ), &vertexArrayData[ 0 ].m_position );
	glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof ( Vertex3D_PCT ), &vertexArrayData[ 0 ].m_color );
	glTexCoordPointer( 2, GL_FLOAT, sizeof( Vertex3D_PCT ), &vertexArrayData[ 0 ].m_texCoords );

	glDrawArrays( GetOpenGLVertexGroupingRule( vertexGroupingRule ), 0, vertexArraySize );

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	UnbindTexture();
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::BindTexture( const Texture* texture )
{
	glEnable( GL_TEXTURE_2D );

	unsigned int paramTextureID = texture->GetTextureID();

	//if ( m_currentTextureID == paramTextureID ) return; //What makes this break things when uncommented out?
	
	glBindTexture( GL_TEXTURE_2D, paramTextureID );
	m_currentTextureID = paramTextureID;
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::UnbindTexture() //Actually just textured drawing with white 1x1 pixel for texture.
{
	BindTexture( m_defaultTexture );
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DrawSphere( const int vertexGroupingRule, const Vector3& centerPos, float radius, float numSides, const Rgba& tint /*= Rgba()*/, float lineThickness /*= 1.0f*/ )
{
	//TODO: BE SMARTER. Figure out how people actually render spheres. Clearly not spherical if looked at down a non-major axis!

	UnbindTexture();

	glLineWidth( lineThickness );

	std::vector<Vertex3D_PCT> vertexes;
	
	//Invoke DrawPolygon's logic along various interpolating heights.

	const float degreesTotal = 360.f;
	const float degreesPerSide = degreesTotal / numSides;
	const float degreesOffset = 0.f;
	float x, y, z;

	//Build 2D circle in YZ at currentX.
	for ( float degrees = 0.f; degrees < degreesTotal; degrees += degreesPerSide )
	{
		y = centerPos.y + ( radius * CosDegrees( degrees ) );
		z = centerPos.z + ( radius * SinDegrees( degrees ) );

		if ( degrees != 0.f ) //Else would add an extra initial edge from first vertex to itself.
			vertexes.push_back( Vertex3D_PCT( Vector3( centerPos.x, y, z ), tint ) );

		vertexes.push_back( Vertex3D_PCT( Vector3( centerPos.x, y, z ), tint ) ); //For drawing AS_LINES.
	}
	//Connect back to 2D circle's starting vertex.
	y = centerPos.y + ( radius * CosDegrees( 0.f ) );
	z = centerPos.z + ( radius * SinDegrees( 0.f ) );
	vertexes.push_back( Vertex3D_PCT( Vector3( centerPos.x, y, z ), tint ) );

	//Build 2D circle in XZ at currentY.
	for ( float degrees = 0.f; degrees < degreesTotal; degrees += degreesPerSide )
	{
		x = centerPos.x + ( radius * CosDegrees( degrees ) );
		z = centerPos.z + ( radius * SinDegrees( degrees ) );

		if ( degrees != 0.f ) //Else would add an extra initial edge from first vertex to itself.
			vertexes.push_back( Vertex3D_PCT( Vector3( x, centerPos.y, z ), tint ) );

		vertexes.push_back( Vertex3D_PCT( Vector3( x, centerPos.y, z ), tint ) ); //For drawing AS_LINES.
	}
	//Connect back to 2D circle's starting vertex.
	x = centerPos.x + ( radius * CosDegrees( 0.f ) );
	z = centerPos.z + ( radius * SinDegrees( 0.f ) );
	vertexes.push_back( Vertex3D_PCT( Vector3( x, centerPos.y, z ), tint ) );

	//Build 2D circle in XY at currentZ.
	for ( float degrees = 0.f; degrees < degreesTotal; degrees += degreesPerSide )
	{
		x = centerPos.x + ( radius * CosDegrees( degrees ) );
		y = centerPos.y + ( radius * SinDegrees( degrees ) );

		if ( degrees != 0.f ) //Else would add an extra initial edge from first vertex to itself.
			vertexes.push_back( Vertex3D_PCT( Vector3( x, y, centerPos.z ), tint ) );

		vertexes.push_back( Vertex3D_PCT( Vector3( x, y, centerPos.z ), tint ) ); //For drawing AS_LINES.
	}
	//Connect back to 2D circle's starting vertex.
	x = centerPos.x + ( radius * CosDegrees( 0.f ) );
	y = centerPos.y + ( radius * SinDegrees( 0.f ) );
	vertexes.push_back( Vertex3D_PCT( Vector3( x, y, centerPos.z ), tint ) );


// 
// 	for ( float currentX = sphereMinX; currentX <= sphereMaxX; currentX += stepBetweenRings )
// 	{
// 		float fractionAlongSphere = ( currentX / sphereMaxX );
// 		if ( fractionAlongSphere <= .5f ) scaledRadius = RangeMap( fractionAlongSphere, 0.f, 1.f, 0.f, 2.f*radius );
// 		else scaledRadius = RangeMap( fractionAlongSphere, 0.f, 1.f, 2.f*radius, 0.f );
// 
// 		//Build 2D circle in YZ at currentX.
// 		for ( float degrees = 0.f; degrees < degreesTotal; degrees += degreesPerSide )
// 		{
// 			float y = centerPos.y + ( scaledRadius * CosDegrees( degrees ) );
// 			float z = centerPos.z + ( scaledRadius * SinDegrees( degrees ) );
// 
// 			if ( degrees != 0.f ) //Else would add an extra initial edge from first vertex to itself.
// 				vertexes.push_back( Vertex3D_PCT( Vector3( currentX, y, z ), tint ) );
// 
// 			vertexes.push_back( Vertex3D_PCT( Vector3( currentX, y, z ), tint ) ); //For drawing AS_LINES.
// 		}
// 		//Connect back to 2D circle's starting vertex.
// 		float y = centerPos.y + ( scaledRadius * CosDegrees( 0.f ) );
// 		float z = centerPos.z + ( scaledRadius * SinDegrees( 0.f ) );
// 		vertexes.push_back( Vertex3D_PCT( Vector3( currentX, y, z ), tint ) );
// 	}

// 	float sphereMinY = centerPos.y - radius;
// 	float sphereMaxY = centerPos.y + radius;
// 	for ( float currentY = sphereMinY; currentY <= sphereMaxY; currentY += stepBetweenRings )
// 	{
// 		float fractionAlongSphere = ( currentY / sphereMaxY );
// 		if ( fractionAlongSphere <= .5f ) scaledRadius = RangeMap( fractionAlongSphere, 0.f, 1.f, 0.f, 2.f*radius );
// 		else scaledRadius = RangeMap( fractionAlongSphere, 0.f, 1.f, 2.f*radius, 0.f );
// 
// 		//Build 2D circle in XZ at currentY.
// 		for ( float degrees = 0.f; degrees < degreesTotal; degrees += degreesPerSide )
// 		{
// 			float x = centerPos.x + ( scaledRadius * CosDegrees( degrees ) );
// 			float z = centerPos.z + ( scaledRadius * SinDegrees( degrees ) );
// 
// 			if ( degrees != 0.f ) //Else would add an extra initial edge from first vertex to itself.
// 				vertexes.push_back( Vertex3D_PCT( Vector3( x, currentY, z ), tint ) );
// 
// 			vertexes.push_back( Vertex3D_PCT( Vector3( x, currentY, z ), tint ) ); //For drawing AS_LINES.
// 		}
// 		//Connect back to 2D circle's starting vertex.
// 		float x = centerPos.x + ( scaledRadius * CosDegrees( 0.f ) );
// 		float z = centerPos.z + ( scaledRadius * SinDegrees( 0.f ) );
// 		vertexes.push_back( Vertex3D_PCT( Vector3( x, currentY, z ), tint ) );
// 	}
// 
// 	float sphereMinZ = centerPos.z - radius;
// 	float sphereMaxZ = centerPos.z + radius;
// 	for ( float currentZ = sphereMinZ; currentZ <= sphereMaxZ; currentZ += stepBetweenRings )
// 	{
// 		float fractionAlongSphere = Interpolate( 0.f, 1.f, ( currentZ / sphereMaxZ ) ); //I think we want to scale by a sin or cos of degrees!!!!!
// 		if ( fractionAlongSphere <= .5f ) scaledRadius = RangeMap( fractionAlongSphere, 0.f, 1.f, 0.f, 2.f*radius );
// 		else scaledRadius = RangeMap( fractionAlongSphere, 0.f, 1.f, 2.f*radius, 0.f );
// 
// 		//Build 2D circle in XY at currentZ.
// 		for ( float degrees = 0.f; degrees < degreesTotal; degrees += degreesPerSide )
// 		{
// 			float x = centerPos.x + ( scaledRadius * CosDegrees( degrees ) );
// 			float y = centerPos.y + ( scaledRadius * SinDegrees( degrees ) );
// 
// 			if ( degrees != 0.f ) //Else would add an extra initial edge from first vertex to itself.
// 				vertexes.push_back( Vertex3D_PCT( Vector3( x, y, currentZ ), tint ) );
// 
// 			vertexes.push_back( Vertex3D_PCT( Vector3( x, y, currentZ ), tint ) ); //For drawing AS_LINES.
// 		}
// 		//Connect back to start of circle.
// 		float x = centerPos.x + ( scaledRadius * CosDegrees( 0.f ) );
// 		float y = centerPos.y + ( scaledRadius * SinDegrees( 0.f ) );
// 		vertexes.push_back( Vertex3D_PCT( Vector3( x, y, currentZ ), tint ) );
// 	}

	DrawVertexArray_PCT( vertexGroupingRule, vertexes, vertexes.size( ) ); //AS_LINES is the ideal.
}


//--------------------------------------------------------------------------------------------------------------
void TheRenderer::DrawCylinder( const int vertexGroupingRule, const Vector3& centerPos, float radius, float height, float numSlices, float numSidesPerSlice, const Rgba& tint /*= Rgba()*/, float lineThickness /*= 1.0f*/ )
{
	UnbindTexture();

	glLineWidth( lineThickness );

	std::vector<Vertex3D_PCT> vertexes;
	float heightStep = ceil( 1.f / numSlices );
	float halfHeight = height * 0.5f;

	//Invoke DrawPolygon's logic along various interpolating heights.
	for ( float currentHeight = centerPos.z - halfHeight; currentHeight <= centerPos.z + halfHeight; currentHeight += heightStep )
	{
		const float radiansTotal = fTWO_PI;
		const float radiansPerSide = radiansTotal / numSidesPerSlice;
		const float degreesOffset = 0.f;

		//Build circle at height.
		for ( float radians = 0.f; radians < radiansTotal; radians += radiansPerSide ) {
			float x = centerPos.x + ( radius * cos( radians ) );
			float y = centerPos.y + ( radius * sin( radians ) );
			vertexes.push_back( Vertex3D_PCT( Vector3( x, y, currentHeight ), Vector2::ZERO, tint ) );
		}
	}

	DrawVertexArray_PCT( vertexGroupingRule, vertexes, vertexes.size( ) );
}
