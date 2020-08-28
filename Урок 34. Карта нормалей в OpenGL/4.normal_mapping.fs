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

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{           
     // получаем нормаль из карты нормалей с диапазоном [0,1]
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    // переводим вектор нормали в диапазон [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // данная нормаль находится в касательном пространстве
   
    // получаем диффузный цвет
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
    // фоновая составляющая
    vec3 ambient = 0.1 * color;
    // диффузная составляющая
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // отраженная составляющая
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}

