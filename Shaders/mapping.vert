#version 450
layout (location = 0) in vec3 aPos;


uniform mat4 view;
uniform mat4 projection;

void main()
{
	//gl_Position = projection * view * vec4(aPos, 1.0);
	gl_Position = vec4(aPos.x-0.7,aPos.y-0.7,aPos.z, 1.0);
}