#version 330 core
out vec4 FragColor;
uniform float ourColor;
void main()
{
	FragColor = vec4(1.0f, ourColor, 0.0f, 1.0f);
}

