#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos; 

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // встроенная переменная, указывающая на то, какую грань мы рендерим
        for(int i = 0; i < 3; ++i) // для каждой вершины треугольника
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}