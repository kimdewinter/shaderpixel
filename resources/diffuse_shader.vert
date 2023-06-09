#version 410 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex_coords;

out vec2 interpolated_tex_coords;
out vec3 interpolated_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	interpolated_normal = in_normal;
	interpolated_tex_coords = in_tex_coords;
	gl_Position = projection * view * model * vec4(in_pos, 1.0f);
}
