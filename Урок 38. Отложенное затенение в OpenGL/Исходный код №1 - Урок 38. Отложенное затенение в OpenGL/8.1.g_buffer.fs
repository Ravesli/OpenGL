#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{    
    // Храним вектор позиции фрагмента в первой текстуре g-буфера
    gPosition = FragPos;
	
    // Также храним нормали каждого фрагмента в g-буфере
    gNormal = normalize(Normal);
	
    // И диффузную составляющую цвета каждого фрагмента
    gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
	
    // Сохраним значение интенсивности отраженной составляющей в альфа-компоненте переменной gAlbedoSpec
    gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;
}