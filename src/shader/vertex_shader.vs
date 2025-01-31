#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;
out vec3 pointColor;
uniform float xOffset;
out vec4 pos_out;
void main()
{
	pos_out = vec4(aPos.xyz, 1.0);
	gl_Position = vec4(aPos.x + xOffset, aPos.yz, 1.0f);
	pointColor = aColor;
}

