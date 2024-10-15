#version 330 core

layout(location = 0) out vec4 fragColor;

uniform sampler2D uTexture;

in vec2 vTexCoord;

void main() {
    vec2 tc = vec2(vTexCoord.x, vTexCoord.y);
    fragColor = texture(uTexture, tc);
}
