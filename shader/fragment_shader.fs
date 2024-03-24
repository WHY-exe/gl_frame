#version 330 core
out vec4 FragColor;
in vec3 pointColor;
in vec4 pos_out;
void main()
{
	FragColor = pos_out;
}

