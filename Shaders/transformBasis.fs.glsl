#version 330 core
out vec4 FragColor;

in vec3 ourColor;
// �������걻�Զ���ֵ
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;

void main()
{
    // ������������������������
    vec4 mix1 = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue);
    FragColor = mix(mix1, vec4(ourColor,1.0) , 0.2);
}