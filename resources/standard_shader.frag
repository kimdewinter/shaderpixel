#version 410 core

// position-related
uniform bool u_pos_on;
in vec3 inter_pos;

// normal-related
uniform bool u_normals_on;
in vec3 inter_normal;

// texture-related
uniform bool u_textures_on;
uniform sampler2D u_texture_diffuse1;
in vec2 inter_tex_coords;

// mvp-matrix components
uniform bool u_modelview_on;
uniform mat4 u_modelview;
uniform bool u_projection_on;
uniform mat4 u_projection;

out vec4 o_frag_color;

void main()
{
	if (u_textures_on)
		o_frag_color = texture(u_texture_diffuse1, inter_tex_coords);
	else
		o_frag_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
