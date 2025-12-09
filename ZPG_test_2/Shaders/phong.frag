#version 330 core

#define MAX_LIGHTS 8

struct Light {
    int type;           // 0 = Ambient, 1 = Point, 2 = Directional, 3 = Spotlight
    vec3 position;      // for Point and Spotlight
    vec3 direction;     // for Directional and Spotlight
    vec3 color;
    float intensity;
    float range;        // for Point and Spotlight
    float cutoff;       // for Spotlight, in radians
};

uniform int lightCount;
uniform Light lights[MAX_LIGHTS];
uniform vec3 cameraPosition;
uniform sampler2D texture1;

in vec4 worldPosition;
in vec3 worldNormal;
in vec2 TexCoord;

out vec4 fragColor;

void main() {
    vec3 norm = normalize(worldNormal);
    vec3 viewDir = normalize(cameraPosition - worldPosition.xyz);
    vec3 texColor = texture(texture1, TexCoord).rgb;

    vec3 totalLight = vec3(0.0);

    for(int i = 0; i < lightCount; ++i) {
        vec3 ambient = vec3(0.0);
        vec3 diffuse = vec3(0.0);
        vec3 specular = vec3(0.0);

        if(lights[i].type == 0) 
        {
            // Ambient
            ambient = lights[i].color * lights[i].intensity;
        } 
        else 
        {
            vec3 lightDir;
            float attenuation = 1.0;

            if(lights[i].type == 1 || lights[i].type == 3) 
            {
                lightDir = normalize(lights[i].position - worldPosition.xyz);
                float distance = length(lights[i].position - worldPosition.xyz);
                attenuation = 1.0 - (distance / lights[i].range);
                if(attenuation < 0.0) attenuation = 0.0;
            } 
            else if(lights[i].type == 2) 
            {
                lightDir = normalize(-lights[i].direction);
            }

            //Diffuse
            float diff = max(dot(norm, lightDir), 0.0);
            diffuse = diff * texColor * lights[i].color * lights[i].intensity * attenuation;

            //Specular (Phong)
            vec3 reflectDir = reflect(-lightDir, norm);
            float sp = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

            if (diff <= 0.0)
            {
                sp = 0.0;
            }

            specular = sp * vec3(1.0) * lights[i].intensity * attenuation;

            //Spotlight factor
            if(lights[i].type == 3) 
            {
                float theta = dot(normalize(-lights[i].direction), lightDir);
                if(theta < cos(lights[i].cutoff)) 
                {
                    diffuse = vec3(0.0);
                    specular = vec3(0.0);
                }
                else 
                {
                    diffuse *= theta;
                    specular *= theta;
                }
            }
        }

        totalLight += ambient + diffuse + specular;
    }

    fragColor = vec4(clamp(totalLight, 0.0, 1.0), 1.0);
}
