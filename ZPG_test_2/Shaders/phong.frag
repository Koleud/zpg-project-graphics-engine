        #version 330 core

        in vec4 worldPosition;
        in vec3 worldNormal;

        out vec4 fragColor;

	struct Light {
        vec3 position;
        vec3 color;
        float intensity;
        };
	
	uniform Light light;
        uniform vec3 cameraPosition;

        void main() {
            vec3 norm = normalize(worldNormal);
            vec3 lightDir = normalize(light.position - worldPosition.xyz);
            vec3 viewDir = normalize(cameraPosition - worldPosition.xyz);
            vec3 reflectDir = reflect(-lightDir, norm);

            vec3 ambient = vec3(0.1);

            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * light.color * light.intensity;

            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
            vec3 specular = spec * vec3(1.0);

            fragColor = vec4(ambient + diffuse + specular, 1.0);
        }