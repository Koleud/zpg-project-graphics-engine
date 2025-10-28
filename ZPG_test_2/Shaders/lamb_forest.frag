#version 330 core

#define MAX_LIGHTS 8

in vec4 worldPosition;
in vec3 worldNormal;

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform int lightCount;
uniform Light lights[MAX_LIGHTS];

out vec4 out_Color;

void main(void) {
    vec3 norm = normalize(worldNormal);
    vec3 totalDiffuse = vec3(0.0);
    vec3 ambient = vec3(0.1); // базовая подсветка

    for (int i = 0; i < lightCount; ++i) {
        vec3 lightDir = normalize(lights[i].position - worldPosition.xyz);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color * lights[i].intensity;

        totalDiffuse += diffuse;
    }

    // Ограничиваем итоговый цвет
    vec3 result = clamp(ambient + totalDiffuse, 0.0, 1.0);
    out_Color = vec4(result, 1.0);
}
