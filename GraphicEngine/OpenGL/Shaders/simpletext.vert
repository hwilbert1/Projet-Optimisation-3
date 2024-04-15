#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Texture Coordinates
layout (location = 2) in vec2 aTex;


// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;

// Imports the camera matrix from the main function
uniform mat4 camMatrix;

uniform mat4 transform;

void main()
{
	// Outputs the positions/coordinates of all vertices
	vec4 temp = transform * vec4(aPos, 1.0);
	
	//temp.x = - temp.x;
	//temp.z = - temp.z;


	//temp.y = -temp.y;

	vec4 temp2 = camMatrix * temp;

	gl_Position = camMatrix * temp;

	// Assigns the colors from the Vertex Data to "color"
	color = aColor;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = aTex;
}