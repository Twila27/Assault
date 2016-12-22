#include "Engine/Renderer/RenderCommand.hpp"

#include "Engine/Renderer/TheRenderer.hpp"


//--------------------------------------------------------------------------------------------------------------
std::list< RenderCommand* >* g_theRenderCommands = nullptr;


//--------------------------------------------------------------------------------------------------------------
void RenderCommandPoint::Render()
{
	float sizeScalar = .1f;

	//x in XY.
	g_theRenderer->DrawLine( m_position + ( Vector3( -1.f, -1.f, 0.f ) * sizeScalar ), m_position + ( Vector3( 1.f, 1.f, 0.f ) * sizeScalar ), m_color, m_color, m_lineThickness );
	g_theRenderer->DrawLine( m_position + ( Vector3( -1.f, 1.f, 0.f ) * sizeScalar ), m_position + ( Vector3( 1.f, -1.f, 0.f ) * sizeScalar ), m_color, m_color, m_lineThickness );

	//x in XZ.
	g_theRenderer->DrawLine( m_position + ( Vector3( -1.f, 0.f, -1.f ) * sizeScalar ), m_position + ( Vector3( 1.f, 0.f, 1.f ) * sizeScalar ), m_color, m_color, m_lineThickness );
	g_theRenderer->DrawLine( m_position + ( Vector3( -1.f, 0.f, 1.f ) * sizeScalar ), m_position + ( Vector3( 1.f, 0.f, -1.f ) * sizeScalar ), m_color, m_color, m_lineThickness );

	//x in YZ.
	g_theRenderer->DrawLine( m_position + ( Vector3( 0.f, -1.f, -1.f ) * sizeScalar ), m_position + ( Vector3( 0.f, 1.f, 1.f ) * sizeScalar ), m_color, m_color, m_lineThickness );
	g_theRenderer->DrawLine( m_position + ( Vector3( 0.f, -1.f, 1.f ) * sizeScalar ), m_position + ( Vector3( 0.f, 1.f, -1.f ) * sizeScalar ), m_color, m_color, m_lineThickness );
}


//--------------------------------------------------------------------------------------------------------------
void RenderCommandLine::Render()
{
	g_theRenderer->DrawLine( m_startPos, m_endPos, m_color, m_color, m_lineThickness );
}


//--------------------------------------------------------------------------------------------------------------
void RenderCommandArrow::Render()
{
	g_theRenderer->DrawLine( m_startPos, m_endPos, m_color, m_color, m_lineThickness );

	float arrowScalar = .5f;
	Vector3 arrowOffsets = ( m_endPos - m_startPos ) * arrowScalar;
	Vector3 arrowX = Vector3( m_startPos.x + arrowOffsets.x, m_endPos.y, m_endPos.z );
	Vector3 arrowY = Vector3( m_endPos.x, m_startPos.y + arrowOffsets.y, m_endPos.z );
	Vector3 arrowZ = Vector3( m_endPos.x, m_endPos.y, m_startPos.z + arrowOffsets.z );

	g_theRenderer->DrawLine( m_endPos, arrowX, m_color, m_color, m_lineThickness );
	g_theRenderer->DrawLine( m_endPos, arrowY, m_color, m_color, m_lineThickness );
	g_theRenderer->DrawLine( m_endPos, arrowZ, m_color, m_color, m_lineThickness );
}


//--------------------------------------------------------------------------------------------------------------
void RenderCommandAABB3::Render()
{
	
	if ( m_drawFilled ) g_theRenderer->DrawAABB( TheRenderer::VertexGroupingRule::AS_QUADS, m_bounds, m_color, m_lineThickness );
	g_theRenderer->DrawAABB( TheRenderer::VertexGroupingRule::AS_LINE_STRIP, m_bounds, m_edgeColor, m_lineThickness * 1.5f ); //Ensure lines show over faces.
}


//--------------------------------------------------------------------------------------------------------------
void RenderCommandSphere::Render()
{
	g_theRenderer->DrawSphere( TheRenderer::VertexGroupingRule::AS_LINES, m_centerPos, m_radius, 10.f, m_color, m_lineThickness );
}


//--------------------------------------------------------------------------------------------------------------
void RenderCommandBasis::Render()
{
	g_theRenderer->DrawDebugAxes( m_axisLength, m_lineThickness, m_showZ );
}


//--------------------------------------------------------------------------------------------------------------
void RenderAndExpireDebugCommands() //Handles the depth modes.
{
	auto commandIterEnd = g_theRenderCommands->end();
	for ( auto commandIter = g_theRenderCommands->begin(); commandIter != commandIterEnd; )
	{
		RenderCommand* currentCommand = *commandIter;

		switch ( currentCommand->m_depthMode )
		{
		case DEPTH_TEST_ON: 
			g_theRenderer->EnableDepthTesting( true );
			currentCommand->Render();
			break;
		case DEPTH_TEST_OFF:
			g_theRenderer->EnableDepthTesting( false );
			currentCommand->Render();
			break;
		case DEPTH_TEST_DUAL:
			unsigned char alphaBackup = currentCommand->m_color.alphaOpacity;
			float sizeBackup = currentCommand->m_lineThickness;
			g_theRenderer->EnableDepthTesting( false );
			currentCommand->m_color.alphaOpacity >>= 2; //Halved.
			currentCommand->m_lineThickness *= .3f;
			currentCommand->Render();

			g_theRenderer->EnableDepthTesting( true );
			currentCommand->m_color.alphaOpacity = alphaBackup;
			currentCommand->m_lineThickness = sizeBackup;
			currentCommand->Render();
			break;
		}

		if ( currentCommand->IsExpired() ) //Expire after draw or 1-frame commands wouldn't show.
		{
			commandIter = g_theRenderCommands->erase( commandIter );

			delete currentCommand;
			currentCommand = nullptr;
		}
		else ++commandIter;
	}
}


//--------------------------------------------------------------------------------------------------------------
void UpdateDebugCommands( float deltaSeconds )
{
	auto commandIterEnd = g_theRenderCommands->end();
	for ( auto commandIter = g_theRenderCommands->begin(); commandIter != commandIterEnd; ++commandIter )
	{
		RenderCommand* currentCommand = *commandIter;
		currentCommand->Update( deltaSeconds );
	}
}


//--------------------------------------------------------------------------------------------------------------
void ClearDebugCommands() //Else program could shutdown before all commands expire.
{
	auto commandIterEnd = g_theRenderCommands->end();
	for ( auto commandIter = g_theRenderCommands->begin(); commandIter != commandIterEnd; )
	{
		RenderCommand* currentCommand = *commandIter;

		if ( currentCommand != nullptr ) //Expire after draw or 1-frame commands wouldn't show.
		{
			commandIter = g_theRenderCommands->erase( commandIter );

			delete currentCommand;
			currentCommand = nullptr;
		}
		else ++commandIter;
	}
}


//--------------------------------------------------------------------------------------------------------------
void AddDebugRenderCommand( RenderCommand* newCommand )
{
	g_theRenderCommands->push_back( newCommand );
}
