#version 400 //330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;

out vec3 Position;
out vec3 Normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 transform;

void main()
{
Normal = mat3(transpose(inverse(ModelMatrix))) * VertexNormal;
Position = vec3(ModelMatrix * vec4(VertexPosition, 1.0));
gl_Position = transform * vec4(VertexPosition, 1.0);
}

// 

//#version 330 core
//layout (location = 0) in vec3 aPos;
//layout (location = 2) in vec3 aNormal;
//out vec3 Normal;
//out vec3 Position;
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//void main()
//{
  //  Normal = mat3(transpose(inverse(model))) * aNormal;
   // Position = vec3(model * vec4(aPos, 1.0));
   /// gl_Position = projection * view * model * vec4(aPos, 1.0);
//}*/