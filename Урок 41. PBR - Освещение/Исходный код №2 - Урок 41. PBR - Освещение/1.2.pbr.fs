#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// параметры материала
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

// освещение
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 camPos;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
// Простой трюк для получения касательного вектора и нормалей в мировом пространстве для упрощения кода PBR.
// Не волнуйтесь, если вы не до конца поняли данный фрагмент кода;  
// я планирую сделать заметку об этой технике 
// где-нибудь позже в учебнике по normal mapping.
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
void main()
{		
    vec3 albedo     = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));
    float metallic  = texture(metallicMap, TexCoords).r;
    float roughness = texture(roughnessMap, TexCoords).r;
    float ao        = texture(aoMap, TexCoords).r;

    vec3 N = getNormalFromMap();
    vec3 V = normalize(camPos - WorldPos);

    // вычисляем коэффициент отражения при перпендикулярном угле падения; в случае диэлектрика (например, пластик) - берем значение F0 
    // равным 0.04, а если металл - используем цвет альбедо (принцип металличности)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // уравнение отражения
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; ++i) 
    {
        // вычисляем энергетическую яркость каждого источника света
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        // BRDF Кука-Торренса 
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 nominator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // прибавляем 0.001 чтобы избежать деления на ноль.
        vec3 specular = nominator / denominator;
        
        // kS эквивалентно коэффициенту Френеля
        vec3 kS = F;
        // чтобы выполнялся закон сохранения энергии, сумма энергий диффузной и отраженной составляющих света не может
        // быть больше 1.0 (кроме тех случаев, когда сама поверхность имеет возможность излучать свет); для выполнения
        // данного соотношения диффузная составляющая (kD) должна равняться значению 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // умножаем kD на значение "1 - металличность", чтобы только неметаллы 
        // имели диффузное освещение, или линейную композицию в случае полуметаллического материала (чисто металлические материалы
        // не имеют диффузного освещения).
        kD *= 1.0 - metallic;	  

        // масштабируем освещенность при помощи NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // добавляем к исходящей энергитической яркости Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // обратите внимани, что мы уже умножали BRDF на коэффициент Френеля(kS), поэтому нам не нужно снова умножать на kS
    }   
    
    // фоновое освещение (обратите внимани, что в следующем уроке (по теме IBL) мы заменим 
    // данную фоновую составляющую освещения на освещение окружающей среды).
    vec3 ambient = vec3(0.03) * albedo * ao;
    
    vec3 color = ambient + Lo;

    // Тональная компрессия HDR 
    color = color / (color + vec3(1.0));
    // Гамма-коррекция
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);
}