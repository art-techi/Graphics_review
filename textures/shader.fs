#version 330 core

uniform sampler2D myTexture; // comes from c++ rather than vertex shader

in vec4 vColor;
in vec2 texCoord; 
out vec4 FragColor;

void main()
{
    FragColor = texture(myTexture, texCoord);
}