#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform float heightScale;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    // Количество слоев глубины
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
	
    // Вычисляем размер каждого слоя
    float layerDepth = 1.0 / numLayers;
	
    // Глубина текущего слоя
    float currentLayerDepth = 0.0;
	
    // Величина сдвига текстурных координат для каждого слоя (из вектора P)
    vec2 P = viewDir.xy / viewDir.z * heightScale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // Получаем начальные значения
    vec2  currentTexCoords = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // Сдвигаем текстурные координаты вдоль направления вектора P
        currentTexCoords -= deltaTexCoords;
		
        // Получаем значение глубины из карты глубины для текущих текстурных координат
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
		
        // Получаем значение глубины следующего слоя
        currentLayerDepth += layerDepth;  
    }
    
    // Получаем текстурные координаты до пересечения
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // Получаем текстурные координаты до и после пересечения (для линейной интерполяции)
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // Интерполяция текстурных координат
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main()
{           
    // Смещение текстурных координат при использовании параллакс-отображения
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = fs_in.TexCoords;
    
    texCoords = ParallaxMapping(fs_in.TexCoords,  viewDir);       
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;

    // Получение нормали из карты нормалей
    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
   
    // Получение диффузного цвета
    vec3 color = texture(diffuseMap, texCoords).rgb;
	
    // Фоновая составляющая
    vec3 ambient = 0.1 * color;
	
    // Диффузная составляющая
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
	
    // Отраженная составляющая    
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}