#version 460 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D baseMap;

void main()
{
    FragColor = texture(baseMap, uv);
} 