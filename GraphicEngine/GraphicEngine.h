#ifndef GRAPHICENGINE_CLASS_H
#define GRAPHICENGINE_CLASS_H


//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------


#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>



#define GL_SILENCE_DEPRECATION

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif


#include <list>
#include<iostream>*

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <stdio.h>

#include "UI/imgui_perso.cpp"
#include "UI/ImGuiOpenGLManager.cpp"


#include <map>






class GraphicException
{
public:
	GraphicException() {}
	GraphicException(const GraphicException&) {}
};

class GraphicEngine
{
public:
	
	std::map<std::string, void (*)()> UIList;

	void (*f1)() ;
	int (*f2)();
	void (*f3)(int, float);

	static GraphicEngine* _graphicengineptr;

	static GraphicEngine* GetInstance()
	{
		if (_graphicengineptr == nullptr)
		{
			_graphicengineptr = new GraphicEngine();
		}

		return _graphicengineptr;
	}

	void Delete();


	void Display(void (*DisplayUIfct)());
	//void Display();


	void Init();


	bool ShouldCloseMainWindow() { return glfwWindowShouldClose(window); }

	unsigned int GetWidth() { return width; }
	unsigned int GetHeight() { return height; }




	



private:


	void InitUI();
	void InitOpenGL();

	const unsigned int width = 1600;
	const unsigned int height = 800;

	GLFWwindow* window;


};



#endif

