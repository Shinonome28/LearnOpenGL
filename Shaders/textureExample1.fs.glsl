#version 330 core
out vec4 FragColor;

in vec3 ourColor;
// 纹理坐标被自动插值
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;

void main()
{
    // 从纹理对象利用纹理坐标采样
    vec4 mix1 = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue);
    FragColor = mix(mix1, vec4(ourColor,1.0) , 0.2);
}