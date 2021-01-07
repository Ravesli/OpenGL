#version 330 core
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;

const float PI = 3.14159265359;

void main()
{		
    // Вектор мирового пространства используется в качестве нормали касательной плоскости, ориентированной вдоль WorldPos. 
	// Учитывая эту нормаль, вычисляем значение всего входящего излучения окружающей среды. 
	// Результатом этого является излучение света, идущее из направления -Normal, 
	// которое мы используем в PBR-шейдере для выборки значений облученности
    vec3 N = normalize(WorldPos);

    vec3 irradiance = vec3(0.0);   
    
    // Расчет касательного пространства от начала координат
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, N);
    up = cross(N, right);
       
    float sampleDelta = 0.025;
    float nrSamples = 0.0;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            // Переход от сферических координат к декартовым (в касательном пространстве)
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            
			// Переход от касательного пространства к мировому
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N; 

            irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));
    
    FragColor = vec4(irradiance, 1.0);
}

