// naming convention:
// i_name = input from buffered data
// u_name = uniform
// inter_name = interpolated output sent to fragment shader

#version 410 core

// position-related
layout (location = 0) in vec3 i_pos;
out vec3 inter_pos;

// normal-related
layout (location = 1) in vec3 i_normal;
out vec3 inter_normal;

// texture-related
layout (location = 2) in vec2 i_tex_coords;
out vec2 inter_tex_coords;

// mvp-matrix
uniform mat4 u_modelview;
uniform mat4 u_projection;

void main()
{
	gl_Position = u_projection * u_modelview * vec4(i_pos, 1.0f);
    inter_pos = vec3(gl_Position.xyz);
	
	inter_normal = i_normal;
	
	inter_tex_coords = i_tex_coords;
}
