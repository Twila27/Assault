#include "Engine/Renderer/OpenGLExtensions.hpp"

//#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library


// Globals "function pointer" variables (these go in OpenGLExtensions.cpp or similar)
PFNGLGENBUFFERSPROC			glGenBuffers		= nullptr;
PFNGLBINDBUFFERPROC			glBindBuffer		= nullptr;
PFNGLBUFFERDATAPROC			glBufferData		= nullptr;
PFNGLDELETEBUFFERSPROC		glDeleteBuffers		= nullptr;