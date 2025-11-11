        #version 330 core
        layout(location = 0) in vec3 vp;
        layout(location = 1) in vec3 vn;
        layout (location = 2) in vec2 texCoord;             //add

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        out vec4 worldPosition;
        out vec3 worldNormal;
        out vec2 TexCoord;              //add

        void main(void) {
            gl_Position = projection * view * model * vec4(vp, 1.0);
            worldPosition = model * vec4(vp, 1.0);
            worldNormal = mat3(model) * vn;
            TexCoord = texCoord;                //add
        }