#version 410 core

// position-related
uniform bool u_pos_on;
layout (location = 0) in vec3 i_pos;
out vec3 inter_pos;

// normal-related
uniform bool u_normals_on;
layout (location = 1) in vec3 i_normal;
out vec3 inter_normal;

// texture-related
uniform bool u_textures_on;
layout (location = 2) in vec2 i_tex_coords;
uniform sampler2D u_texture_diffuse1;
out vec2 inter_tex_coords;

// mvp-matrix components
uniform bool u_modelview_on;
uniform mat4 u_modelview;
uniform bool u_projection_on;
uniform mat4 u_projection;

void main()
{
    if (u_normals_on)
        inter_normal = i_normal;

    if (u_textures_on)
        inter_tex_coords = i_tex_coords;

    gl_Position =
        (u_projection_on ? u_projection : mat4(1.0f)) *
        (u_modelview_on ? u_modelview : mat4(1.0f)) *
        vec4(i_pos, 1.0f);
    
    inter_pos = vec3(gl_Position.xyz);
}
