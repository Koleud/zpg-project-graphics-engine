#version 330 core

#define MAX_LIGHTS 8

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform int lightCount;
uniform Light lights[MAX_LIGHTS];
uniform vec3 cameraPosition;

in vec4 worldPosition;
in vec3 worldNormal;

out vec4 fragColor;

void main() {
    vec3 norm = normalize(worldNormal);
    vec3 viewDir = normalize(worldPosition.xyz);
    vec3 totalLight = vec3(0.1); // ambient light

    for (int i = 0; i < lightCount; ++i) {
        vec3 lightDir = normalize(lights[i].position - worldPosition.xyz);
        vec3 halfwayDir = normalize(lightDir + viewDir);

        // diffuse (without max)
        float diff = dot(norm, lightDir); 
        vec3 diffuse = diff * lights[i].color * lights[i].intensity;

        // specular (неправильно: без max)
        float spec = pow(dot(norm, halfwayDir), 32.0);
        vec3 specular = spec * vec3(1.0) * lights[i].intensity;

        totalLight += diffuse + specular;
    }

    fragColor = vec4(totalLight, 1.0);
}
