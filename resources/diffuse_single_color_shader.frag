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
uniform vec4 u_light; // unit vector to light source; last float can be used in future to indicate directional light (0.0f) or other light (1.0f)

// mvp-matrix
uniform mat4 u_modelview;
uniform mat4 u_projection;

void main()
{
	float diffuse_light = clamp(dot(v2f_normal, u_light.xyz), 0.0, 1.0);

	// We apply Lambertian diffuse lighting
	// by calculating how much this fragment's normal diverges from u_light
	// which tells us how brightly lit this fragment should be
	float divergence = dot(v2f_normal, u_light.xyz); // swizzle u_light from vec4 to vec3
	float clamped = clamp(divergence, 0.0, 1.0); // fragments on darkly lit side can go into negatives
	o_frag_color = vec4(clamped, clamped, clamped, 1.0);
}
