#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;

void main()
{
    vec3 realLightColor = lightColor;
    if (realLightColor == vec3(0.0))
    {
        realLightColor = vec3(1.0);
    }
    FragColor = vec4(realLightColor, 1.0); // set all 4 vector values to 1.0
}