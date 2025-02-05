#version 400 //330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
 
uniform vec3 cameraPosition;
uniform samplerCube skybox;
 
void main() {
 
 vec3 I = normalize(Position - cameraPosition);
 vec3 R = reflect(I, normalize(Normal));
 FragColor = vec4(texture(skybox, R).rgb, 1.0);
}