#version 330 core
uniform vec4 vertexColor; // comes from c++ rather than vertex shader
in vec4 vColor;
out vec4 FragColor;
void main()
{
    FragColor = vColor + vertexColor;
}