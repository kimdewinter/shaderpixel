#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform bool disable_textures; //at time of writing, this uniform is set in Mesh.cpp(Mesh::draw)

void main()
{    
    if (disable_textures)
		FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	else
		FragColor = texture(texture_diffuse1, TexCoords);
}
