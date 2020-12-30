#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
const float offset = 1.0 / 300.0;  

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // верхний-левый
        vec2( 0.0f,    offset), // верхний-центральный
        vec2( offset,  offset), // верхний-правый
        vec2(-offset,  0.0f),   // центральный-левый
        vec2( 0.0f,    0.0f),   // центральный-центральный
        vec2( offset,  0.0f),   // центральный-правый
        vec2(-offset, -offset), // нижний-левый
        vec2( 0.0f,   -offset), // нижний-центральный
        vec2( offset, -offset)  // нижний-правый    
    );

    float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    FragColor = vec4(col, 1.0);
}