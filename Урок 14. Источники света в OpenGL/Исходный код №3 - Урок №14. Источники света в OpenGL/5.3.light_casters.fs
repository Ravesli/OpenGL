#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    vec3 position;  
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);
    
    // Проверяем, находится ли освещение внутри конуса прожектора
    float theta = dot(lightDir, normalize(-light.direction)); 
    
    if(theta > light.cutOff) // помните, что мы работаем с косинусами углов, а не с самими углами, поэтому используем символ сравнения '>'
    {    
        // Окружающая составляющая
        vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
        
        // Диффузная составляющая 
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
        
        // Отраженная составляющая
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
        
        // Затухание
        float distance = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

        // ambient *= attenuation; // убираем ambient-затухание, так как в противном случае, из-за линейного члена ambient внутри ветви else, на больших расстояниях освещенность внутри прожектора будет меньше, чем снаружи
        diffuse *= attenuation;
        specular *= attenuation;   
            
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    }
    else 
    {
        // Иначе, используем ambient-свет, чтобы вне прожектора сцена не была полностью темной
        FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0);
    }
} 