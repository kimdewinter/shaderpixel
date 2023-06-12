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
in vec2 inter_tex_coords;
out vec4 o_frag_color;

// color
uniform vec4 u_color;

// mvp-matrix
uniform mat4 u_modelview;
uniform mat4 u_projection;

void main()
{
	o_frag_color = u_color;
}
