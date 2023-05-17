#version 410 core
out vec4 fragment_color;

in vec2 interpolated_tex_coords;
in vec3 interpolated_normal;

uniform vec3 light_direction;
uniform bool disable_textures;
uniform sampler2D texture_diffuse1;

void main()
{
	// set initial fragment color
	vec4 fragment_color;
	if (disable_textures)
		fragment_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	else
		fragment_color = texture(texture_diffuse1, interpolated_tex_coords);

	// apply diffuse lighting
	fragment_color *= interpolated_normal * light_direction;
}
