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
uniform vec4 u_color; // RGBA; alpha channel unused at time of writing

// lighting
uniform vec4 u_light_vector; // unit vector TO light source; last float can be used in future to indicate directional light (0.0f) or other light (1.0f)
uniform float u_light_intensity; // intensity of light source

// mvp-matrix
uniform mat4 u_modelview;
uniform mat4 u_projection;

void main()
{
	// We apply basic (Lambertian) diffuse lighting
	// get how much this fragment's normal diverges from u_light_vector
	// which tells us how far this fragment is on the lit/shaded side of the object
	float divergence_multiplier = dot(v2f_normal, u_light_vector.xyz); // swizzle u_light_vector from vec4 to vec3
	divergence_multiplier = clamp(divergence_multiplier, 0.0, 1.0); // multiplier of fragments on shaded side can go into the negative
	
	// brighten/dampen color according to intensity of light source
	vec3 intensitied_color = u_color.xyz * u_light_intensity; // swizzle u_color from vec4 to vec3

	// combine everything to get final color
	o_frag_color = vec4(divergence_multiplier * intensitied_color, 1.0);
}
