#pragma once


//For default arguments.
#include "Engine/Renderer/Vertexes.hpp"
#include "Engine/Math/AABB2.hpp"
#include <string>
#include <vector>


//-----------------------------------------------------------------------------
class TheRenderer;
class BitmapFont;
class Texture;
class AABB3;

//-----------------------------------------------------------------------------
extern TheRenderer* g_theRenderer;


//-----------------------------------------------------------------------------
class TheRenderer
{
public:

	enum VertexGroupingRule { AS_LINES, AS_POINTS, AS_LINE_LOOP, AS_LINE_STRIP, AS_QUADS };

	TheRenderer();
	~TheRenderer();

	//State machine context commands.

	void ClearScreenToColor( const Rgba& colorToClearTo );
	void ClearScreenToColor( float red, float green, float blue );
	void ClearScreenDepthBuffer();

	void EnableDepthTesting( bool flagValue );
	void EnableBackfaceCulling( bool flagValue );
	void EnableAlphaTesting( bool flagValue );
	void SetAlphaFunc( int alphaComparatorFunction, float alphaComparatorValue );
	void SetDrawColor( float red, float green, float blue, float opacity ); 
	void SetLineWidth( float newLineWidth );
	void SetBlendFunc( int sourceBlend, int destinationBlend );
	void SetRenderFlag( int flagNameToSet );
	void SetPointSize( float thickness );
	void BindTexture( const Texture* texture );
	void UnbindTexture();

	void SetOrtho( const Vector2& bottomLeft, const Vector2& topRight );
	void SetPerspective( float fovDegreesY, float aspect, float nearDist, float farDist );
	void TranslateView( const Vector2& translation );
	void TranslateView( const Vector3& translation );
	void RotateViewByDegrees( float degrees, const Vector3& axisOfRotation = Vector3(0.f, 0.f, 1.f) );
	void RotateViewByRadians( float radians, const Vector3& axisOfRotation = Vector3(0.f, 0.f, 1.f) );
	void ScaleView( float uniformScale );
	void PushView();
	void PopView();

	//Drawing commands.

	void DrawPoint( const Vector3& position, float thickness, const Rgba& color = Rgba() );

	void DrawLine( const Vector2& startPos, const Vector2& endPos, const Rgba& startColor = Rgba(), const Rgba& endColor = Rgba(), float lineThickness = 1.f );
	void DrawLine( const Vector3& startPos, const Vector3& endPos, const Rgba& startColor = Rgba(), const Rgba& endColor = Rgba(), float lineThickness = 1.f );

	void DrawAABB( const int vertexGroupingRule, const AABB2& bounds, const Texture& texture, const AABB2& texCoords = AABB2( 0.f, 0.f, 1.f, 1.f ), const Rgba& tint = Rgba(), float lineThickness = 1.f );
	void DrawAABB( const int vertexGroupingRule, const AABB3& bounds, const Texture& texture, const AABB2* texCoords, const Rgba& tint = Rgba(), float lineThickness = 1.f ); //Ptr b/c can't have array of refs.

	void DrawAABB( const int vertexGroupingRule, const AABB2& bounds, const Rgba& color = Rgba(), float lineThickness = 1.f );
	void DrawAABB( const int vertexGroupingRule, const AABB3& bounds, const Rgba& color = Rgba(), float lineThickness = 1.f );

	void DrawShadedAABB( const int vertexGroupingRule, const AABB2& bounds, const Rgba& topLeftColor = Rgba(), const Rgba& topRightColor = Rgba(), const Rgba& bottomLeftColor = Rgba(), const Rgba& bottomRightColor = Rgba(), float lineThickness = 1.f );
	void DrawShadedAABB( const int vertexGroupingRule, const AABB3& bounds, const Rgba& topLeftColor = Rgba(), const Rgba& topRightColor = Rgba(), const Rgba& bottomLeftColor = Rgba(), const Rgba& bottomRightColor = Rgba(), float lineThickness = 1.f );
	
	void DrawQuad( const int vertexGroupingRule, const Vector2& topLeft, const Vector2& topRight, const Vector2& bottomRight, const Vector2& bottomLeft, const Rgba& color = Rgba(), float lineThickness = 1.f );
	void DrawPolygon( const int vertexGroupingRule, const Vector2& centerPos, float radius, float numSides, float degreesOffset, const Rgba& color = Rgba(), float lineThickness = 1.f);
	void DrawSphere( const int vertexGroupingRule, const Vector3& centerPos, float radius, float numSides, const Rgba& tint = Rgba(), float lineThickness = 1.0f ); //AS_LINES is ideal.
	void DrawCylinder( const int vertexGroupingRule, const Vector3& centerPos, float radius, float height, float numSlices, float numSidesPerSlice, const Rgba& tint = Rgba(), float lineThickness = 1.0f );

	void DrawText2D( const Vector2& startBottomLeft, const std::string& asciiText, float cellHeight, const Rgba& tint = Rgba( ), const BitmapFont* font = nullptr, float cellAspect = 1.f, bool drawDropShadow = true );

	void DrawAxes( float length, float lineThickness = 1.f, float alphaOpacity = 1.f, bool drawZ = false );
	void DrawDebugAxes( float length = 1.f, float lineThickness = 1.f, bool drawZ = false );

	void DrawVertexArray_PCT( const int vertexGroupingRule, const std::vector< Vertex3D_PCT >& vertexArrayData, unsigned int vertexArraySize );
	void DrawVertexArray_PCT( const int vertexGroupingRule, const Vertex3D_PCT* vertexArrayData, unsigned int vertexArraySize );

	//VBO commands.
	void CreateVbo( unsigned int& out_vboID ); //Returns ID.
	void UpdateVbo( unsigned int vboID, const Vertex3D_PCT* vertexArrayData, unsigned vertexArraySizeInBytes );
	void BindVbo( unsigned int vboID );
	void DestroyVbo( unsigned int vboID );

	void DrawVbo_PCT( unsigned int vboID, int numVerts, VertexGroupingRule vertexGroupingRule );

private:
	void CreateBuiltInTextures();
	unsigned int GetOpenGLVertexGroupingRule( unsigned int TheRendererVertexGroupingRule ) const;
	BitmapFont* m_defaultFont;
	Texture* m_defaultTexture;
	unsigned int m_currentTextureID;
};