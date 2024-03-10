#version 330 core

out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D texture_diffuse1;

uniform vec4 lightColor;

void main()
{    
//	FragColor = lightColor;
    FragColor = texture(texture_diffuse1, v_TexCoord);
}