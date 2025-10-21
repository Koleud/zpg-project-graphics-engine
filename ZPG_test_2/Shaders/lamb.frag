        #version 330 core
        in vec4 worldPosition;
        in vec3 worldNormal;

	struct Light {
        vec3 position;
        vec3 color;
        float intensity;
        };
	
	uniform Light light;

        out vec4 out_Color;
       	void main(void) {
            vec3 lightPosition = vec3(0.0, 0.0, 0.0);
            vec3 lightToVector = light.position - worldPosition.xyz;
            float dotProduct = max(dot(normalize(lightToVector), normalize(worldNormal)), 0.0);
            vec3 diffuse = dotProduct * light.color * light.intensity;
            vec3 ambient = vec3(0.1, 0.1, 0.1);
            out_Color = vec4(ambient + diffuse, 1.0);
        }