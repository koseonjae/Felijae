#version 330

struct Uniforms
{
    mat4 uWorldMat;
    mat4 uViewMat;
    mat4 uProjMat;
    vec3 uCameraPosition;
    vec3 uLightDir;
    vec3 uLightColor;
    vec3 uEmitLight;
};

uniform Uniforms uniforms;

uniform sampler2D uTexture;

in vec3 v_normal;
in vec3 v_viewDir;
in vec2 v_texCoord;
layout(location = 0) out vec4 fragColor;

void main()
{
    vec3 normal = normalize(v_normal);
    vec3 view = normalize(v_viewDir);
    vec3 light = normalize(uniforms.uLightDir);
    vec3 reflectDir = reflect(-light, normal);
    vec3 diffuseColor = texture(uTexture, v_texCoord).xyz;
    float ambientConstant = 0.100000001490116119384765625;
    vec3 ambient = diffuseColor * ambientConstant;
    vec3 diffuse = diffuseColor * max(dot(light, normal), 0.0);
    float specularShiness = 10.0;
    vec3 specular = uniforms.uLightColor * pow(max(dot(reflectDir, view), 0.0), specularShiness);
    vec3 emit = uniforms.uEmitLight;
    vec3 color = ((ambient + diffuse) + specular) + emit;
    fragColor = vec4(color, 1.0);
}