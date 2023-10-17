#version 330 core

in vec2 v_TexCoord;

out vec4 FragColor;
uniform sampler2D u_Texture;

void main() {
    vec4 texColor = texture(u_Texture, v_TexCoord);
    FragColor = texColor;//vec4(1.0f, 0.5f, 0.2f, 1.0f);  // orange color
}
