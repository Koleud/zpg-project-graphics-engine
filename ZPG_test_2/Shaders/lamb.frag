#version 330 core

#define MAX_LIGHTS 8

in vec4 worldPosition;
in vec3 worldNormal;
in vec2 TexCoord;

struct Light {
    int type;           // 0=Ambient, 1=Point, 2=Directional, 3=Spotlight
    vec3 position;      // for Point and Spotlight
    vec3 direction;     // for Directional and Spotlight
    vec3 color;
    float intensity;
    float range;        // for Point and Spotlight
    float cutoff;       // for Spotlight, in radians
};

uniform int lightCount;
uniform Light lights[MAX_LIGHTS];
uniform sampler2D texture1;

out vec4 out_Color;

void main(void) {
    vec3 norm = normalize(worldNormal);

    vec3 texColor = texture(texture1, TexCoord).rgb;

    vec3 baseColor = texColor * 0.05;  
    vec3 totalLight = baseColor;

    for (int i = 0; i < lightCount; ++i) {
        if (lights[i].type == 0) { 
            // Ambient
            totalLight += lights[i].color * lights[i].intensity;
        }
        else if (lights[i].type == 1) { 
            // Point light
            vec3 lightVec = lights[i].position - worldPosition.xyz;
            float distance = length(lightVec);
            if (distance < lights[i].range) {
                vec3 lightDir = normalize(lightVec);
                float diff = max(dot(norm, lightDir), 0.0);
                float attenuation = 1.0 - (distance / lights[i].range);
                totalLight += texColor * diff * lights[i].color * lights[i].intensity * attenuation;
            }
        }
        else if (lights[i].type == 2) { 
            // Directional light
            vec3 lightDir = normalize(-lights[i].direction);
            float diff = max(dot(norm, lightDir), 0.0);
            totalLight += texColor * diff * lights[i].color * lights[i].intensity;
        }
        else if (lights[i].type == 3) { 
            // Spotlight
            vec3 lightVec = lights[i].position - worldPosition.xyz;
            float distance = length(lightVec);
            if (distance < lights[i].range) {
                vec3 lightDir = normalize(lightVec);
                float theta = dot(normalize(-lights[i].direction), lightDir);
                if (theta > cos(lights[i].cutoff)) {
                    float diff = max(dot(norm, lightDir), 0.0);
                    float attenuation = 1.0 - (distance / lights[i].range);
                    totalLight += texColor * diff * lights[i].color * lights[i].intensity * attenuation * theta;
                }
            }
        }
    }

    vec3 result = clamp(totalLight, 0.0, 1.0);
    out_Color = vec4(result, 1.0);
}
