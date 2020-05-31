#version 330 core

layout (location = 0) in vec3 aPos; // position var at attr pos 0
layout (location = 1) in vec3 aColor; //colour var at attr pos 1
layout (location = 2) in vec2 aTexCoord;

out vec4 vColor;
out vec2 texCoord;

void main()
{
   gl_Position = vec4(aPos, 1.0); // set pos of vertex
   vColor = vec4(aColor, 1.0); // pass to frag shader
   texCoord = aTexCoord;
}
   