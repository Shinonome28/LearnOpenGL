#version 330 core
out vec4 FragColor;

// 纹理坐标被自动插值
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.3);
}