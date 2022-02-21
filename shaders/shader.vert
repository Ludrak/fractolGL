#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;
layout (location = 2) in vec2 tex_coord;

//uniform float time;
//out float time_val;

out  vec3 color;
out  vec2 coord;

void main()
{
    color = col;
    coord = tex_coord;

    gl_Position = vec4(pos.xyz, 1.0f);
}