#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader     must be same name as output in vertex shader!!!
in vec4 color;


void main()
{
	FragColor = vec4(color);
}