#version 330 core

layout(location = 0) out vec4 fragColor;

uniform sampler2D uTexture;

in vec2 vTexCoord;

void main() {
    // opengl의 텍스쳐 좌표   : 좌하단이 0,0
    // 이미지의 좌표          : 좌상단이 0,0
    // => s는 그대로 쓰고, t는 1-t로 써줘야 정상적으로 렌더링된다
    vec2 tc = vec2(vTexCoord.x, 1.0 - vTexCoord.y);
    fragColor = texture(uTexture, tc);
}
