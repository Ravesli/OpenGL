#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// Текстурный сэмплер
uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, TexCoord);
}

