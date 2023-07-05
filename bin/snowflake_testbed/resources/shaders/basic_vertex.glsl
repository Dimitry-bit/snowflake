#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCord;

out vec2 ourTexCord;

uniform mat4 uMvp;

void main()
{
    gl_Position = uMvp * vec4(aPosition, 0.0f, 1.0f);
    ourTexCord = aTexCord;
}