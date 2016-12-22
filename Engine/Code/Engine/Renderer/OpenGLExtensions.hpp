#pragma once


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLu.h>
#include "Engine/Renderer/glext.h"
#include "Engine/Renderer/wglext.h"


//Include this header by TheRenderer.
extern PFNGLGENBUFFERSPROC		glGenBuffers;
extern PFNGLBINDBUFFERPROC		glBindBuffer;
extern PFNGLBUFFERDATAPROC		glBufferData;
extern PFNGLDELETEBUFFERSPROC	glDeleteBuffers;
