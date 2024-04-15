#include "GraphicEngine.h"

void GraphicEngine::Delete()
{


	// UI cleanup
	CleanUp();
	//

	




	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();

}

void GraphicEngine::InitOpenGL()
{
	glfwInit();
	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create a GLFWwindow object of 800 by 800 pixels, naming it "BasicGraphicEngine"
	window = glfwCreateWindow(width, height, "BasicGraphicEngine", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		glfwTerminate();

		throw GraphicException();
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);


	//allow for tranparency and blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw GraphicException();
	}
	// Enables the Depth Buffer. tell opengl to use depth
	glEnable(GL_DEPTH_TEST);
}


//void GraphicEngine::Display()
void GraphicEngine::Display(void (*DisplayUIfct)())
{
	
	// ____ imgui

	//Create a new frame
	NewFrame();


	//create a ImGui window

	DisplayUIfct();

	//render Imgui
	RenderFrame();


	// Specify the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render the 3D objects
	RenderFrameData();
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);
	// Take care of all GLFW events
	glfwPollEvents();
}

float temp = 0;






void GraphicEngine::Init()
{

	// Generates Shader object using shaders default.vert and default.frag
//	Shader shaderProgramTextureBG("GraphicEngine/OpenGL/Shaders/default.vert", "OpenGL/Shaders/default.frag");
//	Shader shaderProgramTextureGround("GraphicEngine/OpenGL/Shaders/default.vert", "OpenGL/Shaders/default.frag");


	try
	{
		InitOpenGL();
	}
	catch(GraphicException e)
	{
		//Error
	}

	InitUI();


		
	//RemoveElementFromDisplay(&GroundObj);
}


void GraphicEngine::InitUI()
{
	// ______________________________     ImGui Setup

	const char* glsl_version = "#version 130";

	ImGuiIO& io = ImGuiSetup(window, glsl_version);
	// Our states
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


}
// ---------------------------------------------------------------- 