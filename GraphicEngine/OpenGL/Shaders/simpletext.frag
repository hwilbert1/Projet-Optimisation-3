#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader     must be same name as output in vertex shader!!!
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the Texture Unit from the main function,
uniform sampler2D tex0;



//uniform sampler2D texture1;
//uniform sampler2D texture2;


void main()
{
	FragColor = texture(tex0, texCoord);

	// linearly interpolate between both textures (80% container, 20% awesomeface) mix 2 texture
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}