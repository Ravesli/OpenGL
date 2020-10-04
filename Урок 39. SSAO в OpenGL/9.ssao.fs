#version 330 core
out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

// параметры (вы, вероятно, захотите использовать их в качестве uniform-переменных, чтобы иметь возможность проще настраивать эффект)
int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

// замощаем экран текстурой шума, в соответствии с размерами экрана, делёнными на “размер шума”
const vec2 noiseScale = vec2(800.0/4.0, 600.0/4.0); 

uniform mat4 projection;

void main()
{
    // получаем входные данные для алгоритма SSAO
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);
    // создаем TBN-матрицу смены базиса: из касательного пространства в пространство вида
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // проходится по всем точкам выборки ядра и считаем коэффициент затенения
    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // получаем позицию точки выборки
        vec3 sample = TBN * samples[i]; // from tangent to view-space
        sample = fragPos + sample * radius; 
        
        // проецируем координаты точки выборки (для получения позиции на экране/текстуре)
        vec4 offset = vec4(sample, 1.0);
        offset = projection * offset; // из пространства вида в отсеченное пространство
        offset.xyz /= offset.w; // деление перспективы
        offset.xyz = offset.xyz * 0.5 + 0.5; // приведение к диапазону 0.0 - 1.0
        
        // получение значения глубины точки выборки
        float sampleDepth = texture(gPosition, offset.xy).z; // получение значения глубины точки выборки ядра
        
        // проверка диапазонна и суммирование
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;           
    }
    occlusion = 1.0 - (occlusion / kernelSize);
    
    FragColor = occlusion;
}