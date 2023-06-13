// naming convention:
// i_name = input from buffered data
// u_name = uniform
// v2f_name = interpolated output sent to fragment shader

#version 410 core

// position-related
layout (location = 0) in vec3 i_pos;
out vec3 v2f_pos;

// normal-related
layout (location = 1) in vec3 i_normal;
out vec3 v2f_normal;

// texture-related
uniform sampler2D u_texture_diffuse1;
layout (location = 2) in vec2 i_tex_coords;
out vec2 v2f_tex_coords;

// mvp-matrix
uniform mat4 u_modelview;
uniform mat4 u_projection;

void main()
{
	gl_Position = u_projection * u_modelview * vec4(i_pos, 1.0f);
    v2f_pos = vec3(gl_Position.xyz);
	
	v2f_normal = i_normal;
	
	v2f_tex_coords = i_tex_coords;
}
