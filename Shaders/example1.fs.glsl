#version 330 core
out vec4 FragColor;  
in vec3 color;

uniform vec4 ourColor;

void main()
{
    FragColor = vec4(color, 1.0) + ourColor;
}