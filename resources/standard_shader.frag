// naming convention:
// inter_name = interpolated input sent from vertex shader
// u_name = uniform
// o_name = output

#version 410 core

// position-related
in vec3 inter_pos;

// normal-related
in vec3 inter_normal;

// texture_related
uniform bool u_textures_on;
uniform sampler2D u_texture_diffuse1;
in vec2 inter_tex_coords;
out vec4 o_frag_color;

// mvp-matrix
uniform mat4 u_modelview;
uniform mat4 u_projection;

void main()
{
	if (u_textures_on)
		o_frag_color = texture(u_texture_diffuse1, inter_tex_coords);
	else
		o_frag_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
