#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 tex;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * vec4(aPos, 1.0);
	tex = aTex;
}