#version 330 core

out vec4 outColor;
in vec2 ourTexCord;

uniform vec4 uColor;
uniform sampler2D uTexture0;

void main()
{
    outColor = texture(uTexture0, ourTexCord) * uColor;
}