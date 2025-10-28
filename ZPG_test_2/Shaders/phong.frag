#version 330 core

#define MAX_LIGHTS 8

in vec4 worldPosition;
in vec3 worldNormal;

out vec4 fragColor;

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform int lightCount;
uniform Light lights[MAX_LIGHTS];
uniform vec3 cameraPosition;

void main() {
    vec3 norm = normalize(worldNormal);
    vec3 viewDir = normalize(cameraPosition - worldPosition.xyz);

    vec3 ambient = vec3(0.15); // постоянная слабая подсветка
    vec3 diffuseTotal = vec3(0.0);
    vec3 specularTotal = vec3(0.0);

    for (int i = 0; i < lightCount; ++i) {
        vec3 lightDir = normalize(lights[i].position - worldPosition.xyz);
        vec3 reflectDir = reflect(-lightDir, norm);

        // --- Diffuse ---
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color * lights[i].intensity;

        // --- Specular ---
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = spec * vec3(1.0) * lights[i].intensity;

        // --- Суммируем вклад каждого света ---
        diffuseTotal += diffuse;
        specularTotal += specular;
    }
    diffuseTotal = clamp(diffuseTotal, 0.0, 1.0);
    vec3 result = ambient + diffuseTotal + specularTotal;
    fragColor = vec4(result, 1.0);
}
