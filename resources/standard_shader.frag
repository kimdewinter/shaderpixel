#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform bool disable_textures;

void main()
{    
    if (disable_textures)
		FragColor = vec3(1.0f, 0.0f, 0.0f);
	else
		FragColor = texture(texture_diffuse1, TexCoords);
}
