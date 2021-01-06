#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main()
{    
    // Храним вектор позиции фрагмента в первой текстуре g-буфера
    gPosition = FragPos;
	
    // Также храним нормали каждого фрагмента в g-буфере
    gNormal = normalize(Normal);
	
    // И диффузную составляющую цвета каждого фрагмента
    gAlbedo.rgb = vec3(0.95);
}