// naming convention:
// v2f_name = interpolated input sent from vertex shader
// u_name = uniform
// o_name = output

#version 410 core

// position-related
in vec3 v2f_pos;

// normal-related
in vec3 v2f_normal;

// texture_related
in vec2 v2f_tex_coords;
out vec4 o_frag_color;

// color
uniform vec4 u_color;

// lighting
uniform vec4 u_light; // unit vector to light; last float can be used in future to indicate directional light (0.0f) or other light (1.0f)

// mvp-matrix
uniform mat4 u_modelview;
uniform mat4 u_projection;

void main()
{
	// o_frag_color = u_color;
	// vec4 local_color = u_color; // to prevent unused uniform complaints

	// o_frag_color = vec4(v2f_normal.xyz, 1.0f); // <- uniform not found?

	float diffuse_light = clamp(dot(v2f_normal, u_light.xyz), 0.0, 1.0);
	o_frag_color = vec4(diffuse_light, diffuse_light, diffuse_light, 1.0f);
}
