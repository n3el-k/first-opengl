#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_texCoords;

uniform mat4 transform;

out vec2 texCoords;

void main()
{
    gl_Position = transform * vec4(in_pos, 1.0f);
    texCoords = in_texCoords;
}