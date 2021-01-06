#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// Параметры материала
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

// Освещение
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 camPos;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); // предотвращаем деление на ноль при roughness=0.0 и NdotH=1.0
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{		
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);

    // Вычисляем коэффициент отражения при перпендикулярном угле падения; в случае диэлектрика (например, пластик) - берем значение F0 равным 0.04, 
	// а если металл, то используем цвет альбедо (принцип металличности)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // Уравнение отражения
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; ++i) 
    {
        // Вычисляем энергетическую яркость каждого источника света
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        // BRDF Кука-Торренса 
        float NDF = DistributionGGX(N, H, roughness);   
        float G = GeometrySmith(N, V, L, roughness);      
        vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
           
        vec3 nominator = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = nominator / max(denominator, 0.001); // предотвращаем деление на ноль при NdotV=0.0 or NdotL=0.0
        
        // kS эквивалентно коэффициенту Френеля
        vec3 kS = F;
		
        // Чтобы выполнялся закон сохранения энергии, сумма энергий диффузной и отраженной составляющих света не может быть больше 1.0 
		// (кроме тех случаев, когда сама поверхность имеет возможность излучать свет); 
		// для выполнения данного соотношения диффузная составляющая (kD) должна равняться значению 1.0 - kS
        vec3 kD = vec3(1.0) - kS;
		
        // Умножаем kD на значение "1 - металличность", чтобы только неметаллы имели диффузное освещение, 
		// или линейную композицию в случае полуметаллического материала (чисто металлические материалы не имеют диффузного освещения)
        kD *= 1.0 - metallic;	  

        // Масштабируем освещение коэффициентом NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // Добавляем к исходящей энергитической яркости Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; // обратите внимание, что мы уже умножали BRDF на коэффициент Френеля(kS), поэтому нам не нужно снова умножать на kS
    }   
    
    // Фоновое освещение 
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    // Тональная компрессия HDR 
    color = color / (color + vec3(1.0));
    
	// Гамма-коррекция
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);
}