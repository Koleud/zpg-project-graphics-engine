//Include GLEW
#include <GL/glew.h>

//Include GLFW  
#include <GLFW/glfw3.h>  

//Include GLM  
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

//Include the standard C++ headers  
#include <stdlib.h>
#include <stdio.h>
#include <vector>

//Include tiny_obj_loader
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION

//Include the shader class
#include "Shader.h"
#include "ShaderProgram.h"
#include "Application.h"
#include "Model.h"
#include "DrawableObject.h"
#include "Scene.h"
#include "Translate.h"
#include "Rotate.h"
#include "Scale.h"
#include "Transformation.h"
#include "CompositeTransformation.h"
#include "Camera.h"
#include <random>
#include "Models/tree.h"
#include "Models/bushes.h"
#include "Models/plain.h"
#include "Models/sphere.h"
#include "Models/cube.h"
#include "Light.h"
#include "FileUtils.h"
#include "DynamicRotate.h"
#include "DynamicTranslate.h"
#include "Texture.h"
#include "BezierCurves.h"
#include "BezierSpline.h"
#include "Material.h"

const float PI = 3.14159265f;

int scene1_initialized = 0;
int scene2_initialized = 0;
int scene3_initialized = 1;
int scene4_initialized = 0;
int scene5_initialized = 0;
int scene6_initialized = 0;

bool flashlight_on = false;

static void error_callback(int error, const char* description) { fputs(description, stderr); }

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));

bool rightMousePressed = false;
bool leftMousePressed = false;
bool zKeyPressed = false;
bool qKeyPressed = false;
bool gKeyPressed = false;
bool hKeyPressed = false;

bool arrowUpPressed = false;
bool arrowDownPressed = false;
bool arrowLeftPressed = false;
bool arrowRightPressed = false;

double lastX = 0.0;
double lastY = 0.0;
float yaw = -90.0f;
float pitch = 0.0f;

glm::vec3 worldPos;

BezierSpline bezierSpline;

int stecilIndex = -1;
int selectedObjectIndex = -1;

void OwnUnProject(GLFWwindow* window, Camera& camera, glm::vec3& worldPos)
{
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    int x = (int)mouseX;
    int y = (int)mouseY;

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    int newy = windowHeight - y;

    GLbyte color[4];
    GLfloat depth;
    GLuint index;

    glReadPixels(x, newy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    glReadPixels(x, newy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glReadPixels(x, newy, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

    glm::vec3 screenPos = glm::vec3(x, newy, depth);
    glm::vec4 viewport(0, 0, windowWidth, windowHeight);

    worldPos = glm::unProject(
        screenPos,
        camera.GetViewMatrix(),
        camera.GetProjectionMatrix(),
        viewport
    );

    printf("World pos = [%f, %f, %f], Stencil ID = %u\n", worldPos.x, worldPos.y, worldPos.z, index);
}



static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);

    if (key == GLFW_KEY_1)
    {
        scene1_initialized = 1;
        scene2_initialized = 0;
        scene3_initialized = 0;
        scene4_initialized = 0;
        scene5_initialized = 0;
        scene6_initialized = 0;
    }
    if (key == GLFW_KEY_2)
    {
        scene1_initialized = 0;
        scene2_initialized = 1;
        scene3_initialized = 0;
        scene4_initialized = 0;
        scene5_initialized = 0;
        scene6_initialized = 0;
    }
    if (key == GLFW_KEY_3)
    {
        scene1_initialized = 0;
        scene2_initialized = 0;
        scene3_initialized = 1;
        scene4_initialized = 0;
        scene5_initialized = 0;
        scene6_initialized = 0;
    }
    if (key == GLFW_KEY_4)
    {
        scene1_initialized = 0;
        scene2_initialized = 0;
        scene3_initialized = 0;
        scene4_initialized = 1;
        scene5_initialized = 0;
        scene6_initialized = 0;
    }
    if (key == GLFW_KEY_5)
    {
        scene1_initialized = 0;
        scene2_initialized = 0;
        scene3_initialized = 0;
        scene4_initialized = 0;
        scene5_initialized = 1;
        scene6_initialized = 0;
    }
    if (key == GLFW_KEY_6)
    {
        scene1_initialized = 0;
        scene2_initialized = 0;
        scene3_initialized = 0;
        scene4_initialized = 0;
        scene5_initialized = 0;
        scene6_initialized = 1;
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        flashlight_on = !flashlight_on;
    }
    if (key == GLFW_KEY_Z && action == 1)
    {
        zKeyPressed = true;
        printf("Z key pressed\n");
    }    
    if (key == GLFW_KEY_Q && action == 1)
    {
        OwnUnProject(window, camera, worldPos);
        qKeyPressed = true;
    }
    if (key == GLFW_KEY_UP && action == 1)
    {
        arrowUpPressed = true;
    }
    if (key == GLFW_KEY_DOWN && action == 1)
    {
        arrowDownPressed = true;
    }
    if (key == GLFW_KEY_LEFT && action == 1)
    {
        arrowLeftPressed = true;
    }
    if (key == GLFW_KEY_RIGHT && action == 1)
    {
        arrowRightPressed = true;
    }
    if(key == GLFW_KEY_G && action == 1)
    {
        OwnUnProject(window, camera, worldPos);
        gKeyPressed = true;
    }    
    if (key == GLFW_KEY_H && action == 1)
    {
        //OwnUnProject(window, camera, worldPos);
        hKeyPressed = true;
    }

}

static void window_focus_callback(GLFWwindow* window, int focused) { printf("window_focus_callback \n"); }

static void window_iconify_callback(GLFWwindow* window, int iconified) { printf("window_iconify_callback \n"); }

static void window_size_callback(GLFWwindow* window, int width, int height)
{
    printf("resize %d, %d \n", width, height);
    glViewport(0, 0, width, height);
    float aspect = (float)width / (float)height;

    camera.UpdateProjection(aspect);
}

static void cursor_callback(GLFWwindow* window, double x, double y) 
{ 
    printf("cursor_callback \n"); 
    if (!rightMousePressed) 
    {
        lastX = x;
        lastY = y;
        return;
    }

    float deltaX = x - lastX;
    float deltaY = lastY - y;
    lastX = x;
    lastY = y;

    float sensitivity = 0.3f;
    deltaX *= sensitivity;
    deltaY *= sensitivity;

    yaw += deltaX;
    pitch += deltaY;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    camera.Rotate(yaw, pitch);
}

static void button_callback(GLFWwindow* window, int button, int action, int mode) 
{
	if (action == GLFW_PRESS) 
        printf("button_callback [%d,%d,%d]\n", button, action, mode);

    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
            rightMousePressed = true;
        else if (action == GLFW_RELEASE)
            rightMousePressed = false;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == 1)
    {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        int x = (int)mouseX;
        int y = (int)mouseY;

        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        int newy = windowHeight - y;

        GLuint index;

        glReadPixels(x, newy, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

        printf("World pos = [%f, %f, %f], Stencil ID = %u\n", worldPos.x, worldPos.y, worldPos.z, index);

        stecilIndex = index;
        leftMousePressed = true;
    }
}

void inputCamera(GLFWwindow* window, Camera& camera, float deltaTime)
{
    float speed = 0.001f * deltaTime;
    glm::vec3 dir = camera.GetTargetDirection();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.MoveFoward(dir * speed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.MoveFoward(-dir * speed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.MoveLeft(dir * speed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.MoveLeft(-dir * speed);
}

float randFloat(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

void LoadOBJModel(const std::string& path)
{
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(path)) {
        if (!reader.Error().empty()) {
            printf("TinyOBJLoader error: %s\n", reader.Error().c_str());
        }
        return;
    }

    if (!reader.Warning().empty()) {
        printf("TinyOBJLoader warning: %s\n", reader.Warning().c_str());
    }

    const tinyobj::attrib_t& attrib = reader.GetAttrib();
    const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();
    const std::vector<tinyobj::material_t>& materials = reader.GetMaterials();

    printf("Loaded OBJ with %zu shapes and %zu materials.\n",
        shapes.size(), materials.size());

    if (!attrib.vertices.empty()) {
        printf("First vertex: %.4f, %.4f, %.4f\n",
            attrib.vertices[0], attrib.vertices[1], attrib.vertices[2]);
    }

    if (!shapes.empty()) {
        size_t vertexCount = shapes[0].mesh.indices.size();
        printf("First shape has %zu vertices.\n", vertexCount);
    }
}

Model LoadOBJModelToModel(const std::string& path)
{
    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(path)) {
        throw std::runtime_error(reader.Error());
    }

    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();

    std::vector<float> vertexData;

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            vertexData.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertexData.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertexData.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            if (!attrib.normals.empty())
            {
                vertexData.push_back(attrib.normals[3 * index.normal_index + 0]);
                vertexData.push_back(attrib.normals[3 * index.normal_index + 1]);
                vertexData.push_back(attrib.normals[3 * index.normal_index + 2]);
            }
            else
            {
                vertexData.push_back(0.0f);
                vertexData.push_back(1.0f);
                vertexData.push_back(0.0f);
            }
        }
    }

    size_t vertexCount = vertexData.size() / 6;
    return Model(vertexData, vertexCount);
}

int getSelectedIndex(Scene* scene, int stencilStartIndex)
{
    selectedObjectIndex = stecilIndex - stencilStartIndex;


    for (int i = 0; i < scene->GetObjectCount(); i++)
    {
        if (scene->GetObject(i)->GetID() == stecilIndex)
        {
            selectedObjectIndex = i;
            break;
        }
    }
    return selectedObjectIndex;
}

Application::Application(int width, int height, const char* title)
{
	this->width = width;
	this->height = height;
	this->title = title;
    window = nullptr;
}

Application::~Application()
{
}

void Application::Run()
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(width, height, title, NULL, NULL);


    // OpenGL (3.3 Core Profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(window, &camera); // set the camera as user pointer for unProject callbacks

    // Sets the key callback
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetMouseButtonCallback(window, button_callback);
    glfwSetWindowFocusCallback(window, window_focus_callback);
    glfwSetWindowIconifyCallback(window, window_iconify_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);


    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "ERROR: could not start GLEW\n");
        exit(EXIT_FAILURE);
    }

    // info o OpenGL
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
    printf("Vendor %s\n", glGetString(GL_VENDOR));
    printf("Renderer %s\n", glGetString(GL_RENDERER));
    printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("Using GLFW %i.%i.%i\n", major, minor, revision);

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);


    float points[] = {
        0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f
    };

    float points_с[] = {
       -0.15f, -0.10f, 0.0f, 1,1,0,
        0.15f, -0.10f, 0.0f, 1,1,0,
        0.15f,  0.10f, 0.0f, 1,1,0,
       -0.15f, -0.10f, 0.0f, 1,1,0,
        0.15f,  0.10f, 0.0f, 1,1,0,
       -0.15f,  0.10f, 0.0f, 1,1,0
    };

    float points_с_2[] = {
        // X      Y      Z      R     G     B
        -0.15f, -0.10f, 0.0f,  1.0f, 0.0f, 1.0f,
         0.15f, -0.10f, 0.0f,  0.0f, 1.0f, 1.0f,
         0.15f,  0.10f, 0.0f,  0.0f, 1.0f, 0.5f,

        -0.15f, -0.10f, 0.0f,  1.0f, 0.0f, 1.0f,
         0.15f,  0.10f, 0.0f,  0.0f, 1.0f, 0.5f,
        -0.15f,  0.10f, 0.0f,  1.0f, 0.5f, 0.0f
    };


    //Shaders
 
    //Classic shaders (with color attribute)
    const std::string vertex_shader = ReadFileToString("Shaders\\classic.vert");
    const std::string fragment_shader = ReadFileToString("Shaders\\classic.frag");


    //Constant color shaders (without lighting)
    const std::string vertex_shader_without_color = ReadFileToString("Shaders\\constant.vert");
    const std::string fragment_shader_without_color = ReadFileToString("Shaders\\constant.frag");


    //Phong's shading model shaders
    const std::string vertex_shader_light_2 = ReadFileToString("Shaders\\phong.vert");
    const std::string fragment_shader_light_2 = ReadFileToString("Shaders\\phong.frag");


    //Blinn-Phong's shading model shaders
    const std::string vertex_shader_light_3 = ReadFileToString("Shaders\\blinnphong.vert");
    const std::string fragment_shader_light_3 = ReadFileToString("Shaders\\blinnphong.frag");


    //Lambert's shading model shaders
    const std::string fiona_vertex_shader = ReadFileToString("Shaders\\lamb.vert");
    const std::string fiona_fragment_shader = ReadFileToString("Shaders\\lamb.frag");

    //Lambert's shading model shaders without texture
    const std::string vertex_shader_without_texture = ReadFileToString("Shaders\\lamb_without_tex.vert");
    const std::string fragment_shader_without_texture = ReadFileToString("Shaders\\lamb_without_tex.frag");


    Texture grassTexture("Textures\\grass.jpg");
    Texture shrekTexture("Models\\shrek\\shrek.png");
    Texture fionaTexture("Models\\shrek\\fiona.png");
    Texture f1Texture("Models\\formula_1\\Substance SpecGloss\\Right ones\\formula1_DefaultMaterial_Diffuse.png");
    Texture sunTexture("Textures\\2k_sun.jpg");
    Texture earthTexture("Textures\\2k_earth_daymap.jpg");
    Texture moonTexture("Textures\\2k_moon.jpg");
    Texture skyboxTexture("Textures\\2k_stars_milky_way.jpg");
    shrekTexture.Bind(0);
    fionaTexture.Bind(1);
    grassTexture.Bind(2);
    f1Texture.Bind(3);
    sunTexture.Bind(4);
    earthTexture.Bind(5);
    moonTexture.Bind(6);
    skyboxTexture.Bind(7);


    // create shaders
    Shader vertex_shader_obj(vertex_shader, GL_VERTEX_SHADER);
    Shader fragment_shader_obj(fragment_shader, GL_FRAGMENT_SHADER);

    Shader vertex_shader_without_color_obj(vertex_shader_without_color, GL_VERTEX_SHADER);
    Shader fragment_shader_without_color_obj(fragment_shader_without_color, GL_FRAGMENT_SHADER);

    Shader vertex_shader_light_obj(fiona_vertex_shader, GL_VERTEX_SHADER);
    Shader fragment_shader_light_obj(fiona_fragment_shader, GL_FRAGMENT_SHADER);

	Shader vertex_shader_light_2_obj(vertex_shader_light_2, GL_VERTEX_SHADER);
	Shader fragment_shader_light_2_obj(fragment_shader_light_2, GL_FRAGMENT_SHADER);

    Shader vertex_shader_light_3_obj(vertex_shader_light_3, GL_VERTEX_SHADER);
    Shader fragment_shader_light_3_obj(fragment_shader_light_3, GL_FRAGMENT_SHADER);

    Shader grass_vertex_shader_obj(fiona_vertex_shader, GL_VERTEX_SHADER);
    Shader grass_fragment_shader_obj(fiona_fragment_shader, GL_FRAGMENT_SHADER);

    Shader f1_vertex_shader_obj(vertex_shader_light_2, GL_VERTEX_SHADER);
    Shader f1_fragment_shader_obj(fragment_shader_light_2, GL_FRAGMENT_SHADER);

    Shader shrek_vertex_shader_obj(fiona_vertex_shader, GL_VERTEX_SHADER);
    Shader shrek_fragment_shader_obj(fiona_fragment_shader, GL_FRAGMENT_SHADER);

    Shader fiona_vertex_shader_obj(fiona_vertex_shader, GL_VERTEX_SHADER);
    Shader fiona_fragment_shader_obj(fiona_fragment_shader, GL_FRAGMENT_SHADER);


    Shader sun_vertex_shader_obj(vertex_shader_without_color, GL_VERTEX_SHADER);
    Shader sun_fragment_shader_obj(fragment_shader_without_color, GL_FRAGMENT_SHADER);

    Shader earth_vertex_shader_obj(fiona_vertex_shader, GL_VERTEX_SHADER);
    Shader earth_fragment_shader_obj(fiona_fragment_shader, GL_FRAGMENT_SHADER);

    Shader moon_vertex_shader_obj(fiona_vertex_shader, GL_VERTEX_SHADER);
    Shader moon_fragment_shader_obj(fiona_fragment_shader, GL_FRAGMENT_SHADER);

    Shader skybox_vertex_shader_obj(vertex_shader_without_color, GL_VERTEX_SHADER);
    Shader skybox_fragment_shader_obj(fragment_shader_without_color, GL_FRAGMENT_SHADER);

    Shader vertex_shader_without_texture_obj(vertex_shader_without_texture, GL_VERTEX_SHADER);
    Shader fragment_shader_without_texture_obj(fragment_shader_without_texture, GL_FRAGMENT_SHADER);




    // create shader program
    ShaderProgram shader_program_1(vertex_shader_obj, fragment_shader_obj, &camera);
    ShaderProgram shader_program_without_color(vertex_shader_without_color_obj, fragment_shader_without_color_obj, &camera);
    ShaderProgram shader_program_light(vertex_shader_light_obj, fragment_shader_light_obj, &camera);
	ShaderProgram shader_program_light_2(vertex_shader_light_2_obj, fragment_shader_light_2_obj, &camera);
	ShaderProgram shader_program_light_3(vertex_shader_light_3_obj, fragment_shader_light_3_obj, &camera);

    ShaderProgram grass_shader_program(grass_vertex_shader_obj, grass_fragment_shader_obj, &camera);
    ShaderProgram grass_shader_program_game(grass_vertex_shader_obj, grass_fragment_shader_obj, &camera);

    ShaderProgram f1_shader_program(f1_vertex_shader_obj, f1_fragment_shader_obj, &camera);
    ShaderProgram f1_shader_program_game(f1_vertex_shader_obj, f1_fragment_shader_obj, &camera);


    ShaderProgram shrek_shader_program(shrek_vertex_shader_obj, shrek_fragment_shader_obj, &camera);
    ShaderProgram fiona_shader_program(fiona_vertex_shader_obj, fiona_fragment_shader_obj, &camera);

    ShaderProgram sun_shader_program(sun_vertex_shader_obj, sun_fragment_shader_obj, &camera);
    ShaderProgram earth_shader_program(earth_vertex_shader_obj, earth_fragment_shader_obj, &camera);
    ShaderProgram moon_shader_program(moon_vertex_shader_obj, moon_fragment_shader_obj, &camera);
    ShaderProgram skybox_shader_program(skybox_vertex_shader_obj, skybox_fragment_shader_obj, &camera);

    ShaderProgram shader_program_without_texture(vertex_shader_without_texture_obj, fragment_shader_without_texture_obj, &camera);


	//create scenes
    Scene scene_1 = Scene();
    Scene scene_2 = Scene();
    Scene scene_3 = Scene();
    Scene scene_4 = Scene();
    Scene scene_5 = Scene();
    Scene scene_6 = Scene();




    std::vector<float> sphereWithUV;
    int vertexCount = sizeof(sphere) / (6 * sizeof(float));

    for (int i = 0; i < vertexCount; ++i) {
        float x = sphere[i * 6 + 0];
        float y = sphere[i * 6 + 1];
        float z = sphere[i * 6 + 2];

        float nx = sphere[i * 6 + 3];
        float ny = sphere[i * 6 + 4];
        float nz = sphere[i * 6 + 5];

        // Добавляем позицию
        sphereWithUV.push_back(x);
        sphereWithUV.push_back(y);
        sphereWithUV.push_back(z);

        // Добавляем нормаль
        sphereWithUV.push_back(nx);
        sphereWithUV.push_back(ny);
        sphereWithUV.push_back(nz);

        // Вычисляем UV по сферическим координатам
        glm::vec3 p = glm::normalize(glm::vec3(x, y, z));
        float u = 0.5f + atan2(p.z, p.x) / (2.0f * PI);
        float v = 0.5f - asin(p.y) / PI;

        sphereWithUV.push_back(u);
        sphereWithUV.push_back(v);
    }



	// create models
	Model triangleModel(std::vector<float>(points, points + sizeof(points) / sizeof(float)), sizeof(points) / (6 * sizeof(float)));
	Model treeModel(std::vector<float>(tree, tree + sizeof(tree) / sizeof(float)), sizeof(tree) / (6 * sizeof(float)));
	Model bushesModel(std::vector<float>(bushes, bushes + sizeof(bushes) / sizeof(float)), sizeof(bushes) / (6 * sizeof(float)));
	Model plainModel(std::vector<float>(plain, plain + sizeof(plain) / sizeof(float)), sizeof(plain) / (6 * sizeof(float)), true);
	Model sphereModel(std::vector<float>(sphere, sphere + sizeof(sphere) / sizeof(float)), sizeof(sphere) / (6 * sizeof(float)));
    Model cubeModel(std::vector<float>(cube, cube + sizeof(cube) / sizeof(float)), sizeof(cube) / (6 * sizeof(float)));
    Model formula_1_model("Models\\formula_1\\Formula_1_mesh.obj", "Models\\formula_1\\Formula_1_mesh.mlt");
    Model sphereModelWithUV(sphereWithUV, sphereWithUV.size() / 8, true);
    Model loginModel("Models\\login_vsb\\login_sha0062.obj", "Models\\login_vsb\\login_sha0062.mlt");

	// create drawable objects
	DrawableObject triangle(&shader_program_1, &triangleModel);


    //create four spheres with different shaders
	std::vector<DrawableObject> spheres;
    DrawableObject sphere_1(&shader_program_without_color, &sphereModel);
    spheres.push_back(sphere_1);
    DrawableObject sphere_2(&shader_program_light, &sphereModel);
    spheres.push_back(sphere_2);
    DrawableObject sphere_3(&shader_program_light_2, &sphereModel);
    spheres.push_back(sphere_3);
    DrawableObject sphere_4(&shader_program_light_3, &sphereModel);
    spheres.push_back(sphere_4);


    //create fireflys objects
    std::vector<DrawableObject> fireflys_obj;
    DrawableObject fireflys_1(&shader_program_without_color, &sphereModel);
    DrawableObject fireflys_2(&shader_program_without_color, &sphereModel);
    DrawableObject fireflys_3(&shader_program_without_color, &sphereModel);
    DrawableObject fireflys_4(&shader_program_without_color, &sphereModel);
    DrawableObject fireflys_5(&shader_program_without_color, &sphereModel);
    fireflys_obj.push_back(fireflys_1);
    fireflys_obj.push_back(fireflys_2);
    fireflys_obj.push_back(fireflys_3);
    fireflys_obj.push_back(fireflys_4);
    fireflys_obj.push_back(fireflys_5);
    scene_3.AddObject(&fireflys_obj[0]);
    scene_3.AddObject(&fireflys_obj[1]);
    scene_3.AddObject(&fireflys_obj[2]); 
    scene_3.AddObject(&fireflys_obj[3]);
    scene_3.AddObject(&fireflys_obj[4]);


	// create multiple trees and bushes with random positions and scales
    std::vector<DrawableObject> trees;
	std::vector<DrawableObject> bushes_list;
	for (int i = 0; i < 50; i++)
	{
		DrawableObject tree(&shader_program_without_texture, &treeModel);
        tree.transform.AddTransformation(new Rotate(glm::vec3(0.0f, randFloat(0.0f, 360.0f), 0.0f)));
		tree.transform.AddTransformation(new Translate(glm::vec3(randFloat(-30.0f, 30.0f), 0.0f, randFloat(-30.0f, 30.0f))));
		float scale = randFloat(0.3f, 1.0f);
		tree.transform.AddTransformation(new Scale(glm::vec3(scale, scale, scale)));
		trees.push_back(tree);

		
		DrawableObject bush(&shader_program_without_texture, &bushesModel);
		bush.transform.AddTransformation(new Rotate(glm::vec3(0.0f, randFloat(0.0f, 360.0f), 0.0f)));
		bush.transform.AddTransformation(new Translate(glm::vec3(randFloat(-30.0f, 30.0f), 0.0f, randFloat(-30.0f, 30.0f))));
		float scale_bush = randFloat(0.5f, 1.5f);
		bush.transform.AddTransformation(new Scale(glm::vec3(scale_bush, scale_bush, scale_bush)));
		bushes_list.push_back(bush);
	}
    DrawableObject login_obj(&shader_program_without_texture, &loginModel);
    login_obj.transform.AddTransformation(new Translate(glm::vec3(-5.0f, 0.0f, 0.0f)));
	DrawableObject plain(&grass_shader_program, &plainModel);
	plain.transform.AddTransformation(new Scale(glm::vec3(50.0f, 1.0f, 50.0f)));


    DrawableObject cube(&skybox_shader_program, &sphereModelWithUV);
    

    //create Solar system scene objects
    DrawableObject sun(&sun_shader_program, &sphereModelWithUV);
    DrawableObject earth(&earth_shader_program, &sphereModelWithUV);
    DrawableObject moon(&moon_shader_program, &sphereModelWithUV);
    std::vector<SceneLight> lights_solarSystem;
    std::vector<SceneLight> sun_own_light;
    SceneLight sun_light(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 10.0f);
    SceneLight sun_own(glm::vec3(0.9f, 0.7f, 0.3f), 0.45f);
    lights_solarSystem.push_back(sun_light);
    sun_own_light.push_back(sun_own);

    //create object for game scene
    DrawableObject plain_game(&grass_shader_program_game, &plainModel);
    plain_game.transform.AddTransformation(new Scale(glm::vec3(20.0f, 1.0f, 20.0f)));
    DrawableObject f1(&f1_shader_program_game, &formula_1_model);

    f1.transform.AddTransformation(new Scale(glm::vec3(0.005f)));
    f1.transform.AddTransformation(new Translate(glm::vec3(0.0f, 0.0f, 0.0f)));
    f1.transform.AddTransformation(new DynamicTranslate(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.04f, 0.0f, 0.0f)));

    std::vector<SceneLight> lights_game;
    SceneLight directional_game(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.8f, 0.8f, 0.8f), 0.5f);
    lights_game.emplace_back(directional_game);



    //create object for Bezier curve scene
    DrawableObject plain_bezier(&grass_shader_program, &plainModel);
    plain_bezier.transform.AddTransformation(new Scale(glm::vec3(20.0f, 1.0f, 20.0f)));
    DrawableObject f1_bezier(&f1_shader_program, &formula_1_model);
    f1_bezier.transform.AddTransformation(new Scale(glm::vec3(0.005f)));
    f1_bezier.transform.AddTransformation(new Translate(glm::vec3(0.0f, 0.0f, 0.0f)));
    f1_bezier.transform.AddTransformation(new BezierCurves (glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(5.0f, 0.0f, 5.0f),
            glm::vec3(10.0f, 0.0f, -5.0f),
            glm::vec3(15.0f, 0.0f, 0.0f)
    , 0.001f));


	//add objects to scenes

    //scene 1 - rotating triangle
    scene_1.AddObject(&triangle);


	// scene 2 - four spheres
    for (int i = 0; i < 4; i++)
    {
		scene_2.AddObject(&spheres[i]);
    }


	// scene 3 - forest with plain
	for (int i = 0; i < trees.size(); i++)
	{
		scene_3.AddObject(&trees[i]);
	}
	for (int i = 0; i < bushes_list.size(); i++)
	{
		scene_3.AddObject(&bushes_list[i]);
	}
    scene_3.AddObject(&plain);
    scene_3.AddObject(&login_obj);
    //scene_3.AddObject(&cube);


    // scene 4 - solar system
    scene_4.AddObject(&sun);
    scene_4.AddObject(&earth);
    scene_4.AddObject(&moon);


    // scene 5 - simple game scene
    scene_5.AddObject(&plain_game);
    scene_5.AddObject(&f1);


    // scene 6 - Bezier curve scene
    scene_6.AddObject(&plain_bezier);
    scene_6.AddObject(&f1_bezier);


	// create view and projection matrices0
    glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    //glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 100.0f);
    //glm::mat4 projection = glm::perspective(glm::radians(130.0f), (float)width / (float)height, 0.1f, 100.0f);


	shader_program_1.Use();
	shader_program_1.SetUniform("view", view);
	shader_program_1.SetUniform("projection", projection);

    shader_program_without_color.Use();
    shader_program_without_color.SetUniform("view", view);
    shader_program_without_color.SetUniform("projection", projection);


    //Sphere scene lights
    std::vector<SceneLight> lights;
    SceneLight mainLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.385f, 0.647f, 0.812f), 1.0f, 10.0f);

    lights.emplace_back(mainLight);


    std::vector<SceneLight> fireflys;
    SceneLight mainLight_forest(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.812f, 0.647f, 0.385f), 1.0f, 2.0f);
    SceneLight secondaryLight_forest(glm::vec3(20.0f, 0.5f, 0.0f), glm::vec3(0.812f, 0.647f, 0.385f), 1.0f, 2.0f);
    SceneLight secondaryLight_forest_1(glm::vec3(10.0f, 0.5f, 0.0f), glm::vec3(0.812f, 0.647f, 0.385f), 1.0f, 2.0f);
    SceneLight secondaryLight_forest_2(glm::vec3(15.0f, 0.5f, 0.0f), glm::vec3(0.812f, 0.647f, 0.385f), 1.0f, 2.0f);
    SceneLight secondaryLight_forest_3(glm::vec3(25.0f, 0.5f, 0.0f), glm::vec3(0.812f, 0.647f, 0.385f), 1.0f, 2.0f);
    SceneLight directional_forest(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.3f, 0.3f, 0.3f), 0.5f);
    SceneLight flashlight_forest(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.685f, 0.647f, 0.712f), 0.0f, 21.0f, glm::radians(15.0f));

    DynamicTranslate* dt = new DynamicTranslate(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.0f, 0.0f));

    mainLight_forest.transform.AddTransformation(dt);
    secondaryLight_forest.transform.AddTransformation(dt);
    secondaryLight_forest_1.transform.AddTransformation(new DynamicTranslate(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.0f, 0.01f)));
    secondaryLight_forest_2.transform.AddTransformation(new DynamicTranslate(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.0f)));
    secondaryLight_forest_3.transform.AddTransformation(new DynamicTranslate(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.01f, 0.0f)));

    fireflys.emplace_back(mainLight_forest);
    fireflys.emplace_back(secondaryLight_forest);
    fireflys.emplace_back(secondaryLight_forest_1);
    fireflys.emplace_back(secondaryLight_forest_2);
    fireflys.emplace_back(secondaryLight_forest_3);
    fireflys.emplace_back(directional_forest);
    fireflys.emplace_back(flashlight_forest);

    fireflys_obj[0].transform.AddTransformation(new Scale(glm::vec3(0.05f, 0.05f, 0.05f)));
    fireflys_obj[1].transform.AddTransformation(new Scale(glm::vec3(0.05f, 0.05f, 0.05f)));
    fireflys_obj[2].transform.AddTransformation(new Scale(glm::vec3(0.05f, 0.05f, 0.05f)));
    fireflys_obj[3].transform.AddTransformation(new Scale(glm::vec3(0.05f, 0.05f, 0.05f)));     
    fireflys_obj[4].transform.AddTransformation(new Scale(glm::vec3(0.05f, 0.05f, 0.05f)));

    fireflys_obj[0].transform.AddTransformation(new Translate(glm::vec3(0.0f, 0.5f, 0.0f)));
    fireflys_obj[1].transform.AddTransformation(new Translate(glm::vec3(20.0f, 0.5f, 0.0f)));
    fireflys_obj[2].transform.AddTransformation(new Translate(glm::vec3(10.0f, 0.5f, 0.0f)));
    fireflys_obj[3].transform.AddTransformation(new Translate(glm::vec3(15.0f, 0.5f, 0.0f)));
    fireflys_obj[4].transform.AddTransformation(new Translate(glm::vec3(25.0f, 0.5f, 0.0f)));

    fireflys_obj[0].transform.AddTransformation(dt);
    fireflys_obj[1].transform.AddTransformation(dt);
    fireflys_obj[2].transform.AddTransformation(new DynamicTranslate(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.0f, 0.01f)));
    fireflys_obj[3].transform.AddTransformation(new DynamicTranslate(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.0f)));
    fireflys_obj[4].transform.AddTransformation(new DynamicTranslate(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.01f, 0.0f)));


    DrawableObject formula_1(&f1_shader_program, &formula_1_model);
    formula_1.transform.AddTransformation(new Scale(glm::vec3(0.005f)));
    formula_1.transform.AddTransformation(new Translate(glm::vec3(0.0f, 0.0f, 0.0f)));

    scene_3.AddObject(&formula_1);

    //Model shrek_model = LoadOBJModelToModel("Models\\shrek\\shrek.obj");
    Model shrek_model ("Models\\shrek\\shrek.obj", "Models\\shrek\\shrek.mlt");
    DrawableObject shrek(&shrek_shader_program, &shrek_model);
    Model fiona_model("Models\\shrek\\fiona.obj", "Models\\shrek\\fiona.mlt");
    DrawableObject fiona(&fiona_shader_program, &fiona_model);
    shrek.transform.AddTransformation(new Translate(glm::vec3(3.0f, 0.0f, 0.0f)));
    fiona.transform.AddTransformation(new Translate(glm::vec3(4.5f, 0.0f, 0.0f)));

    scene_3.AddObject(&shrek);
    scene_3.AddObject(&fiona);


    shader_program_light.Use();
    shader_program_light.SetLight(fireflys);
    shader_program_light.SetUniform("view", view);
    shader_program_light.SetUniform("projection", projection);

    f1_shader_program.Use();
    f1Texture.Bind(3);
    f1_shader_program.SetUniform("texture1", 3);
    f1_shader_program.SetLight(fireflys);
    f1_shader_program.SetUniform("view", view);
    f1_shader_program.SetUniform("projection", projection);

    Material redPlastic(
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        32.0f
    );

    f1_shader_program_game.Use();
    f1_shader_program_game.SetLight(lights_game);
    f1_shader_program_game.SetUniform("texture1", 3);
    f1_shader_program_game.SetUniform("view", view);
    f1_shader_program_game.SetUniform("projection", projection);
    f1_shader_program_game.SetUniform("materialAmbient", redPlastic.ambient);
    f1_shader_program_game.SetUniform("materialDiffuse", redPlastic.diffuse);
    f1_shader_program_game.SetUniform("materialSpecular", redPlastic.specular);
    f1_shader_program_game.SetUniform("materialShininess", redPlastic.shininess);

    shrek_shader_program.Use();
    shrekTexture.Bind(0);
    shrek_shader_program.SetUniform("texture1", 0);
    shrek_shader_program.SetLight(fireflys);
    shrek_shader_program.SetUniform("view", view);
    shrek_shader_program.SetUniform("projection", projection);

    fiona_shader_program.Use();
    fionaTexture.Bind(1);
    fiona_shader_program.SetUniform("texture1", 1);
    fiona_shader_program.SetLight(fireflys);
    fiona_shader_program.SetUniform("view", view);
    fiona_shader_program.SetUniform("projection", projection);

    grass_shader_program.Use();
    grassTexture.Bind(2);
    grass_shader_program.SetUniform("texture1", 2);
    grass_shader_program.SetLight(fireflys);
    grass_shader_program.SetUniform("view", view);
    grass_shader_program.SetUniform("projection", projection);

    grass_shader_program_game.Use();
    grass_shader_program_game.SetLight(lights_game);
    grass_shader_program_game.SetUniform("texture1", 2);
    grass_shader_program_game.SetUniform("view", view);
    grass_shader_program_game.SetUniform("projection", projection);

    sun_shader_program.Use();
    //sun_shader_program.SetLight(sun_own_light);
    sun_shader_program.SetUniform("texture1", 4);
    sun_shader_program.SetUniform("view", view);
    sun_shader_program.SetUniform("projection", projection);

    earth_shader_program.Use();
    earth_shader_program.SetLight(lights_solarSystem);
    earth_shader_program.SetUniform("texture1", 5);
    earth_shader_program.SetUniform("view", view);
    earth_shader_program.SetUniform("projection", projection);

    moon_shader_program.Use();
    moon_shader_program.SetLight(lights_solarSystem);
    moon_shader_program.SetUniform("texture1", 6);
    moon_shader_program.SetUniform("view", view);
    moon_shader_program.SetUniform("projection", projection);    
    

    shader_program_light_2.Use();
    shader_program_light_2.SetLight(lights);
	shader_program_light_2.SetUniform("view", view);
	shader_program_light_2.SetUniform("projection", projection);
	shader_program_light_2.SetUniform("cameraPosition", camera.GetPosition());


    shader_program_light_3.Use();
    shader_program_light_3.SetLight(lights);
    shader_program_light_3.SetUniform("view", view);
    shader_program_light_3.SetUniform("projection", projection);
    shader_program_light_3.SetUniform("cameraPosition", camera.GetPosition());

    shader_program_without_texture.Use();
    shader_program_without_texture.SetLight(lights);
    shader_program_without_texture.SetUniform("view", view);
    shader_program_without_texture.SetUniform("projection", projection);
    shader_program_without_texture.SetUniform("cameraPosition", camera.GetPosition());


	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	float angle = 0.0f;
    float angle_2 = 0.0f;

	//some pre-transofrmations
 
    //scene 1 - triangle
    scene_1.GetObject(0)->transform.AddTransformation(new DynamicRotate(glm::vec3(0.0f, 0.0f, 1.0f), 0.01f));
    scene_1.GetObject(0)->transform.AddTransformation(new Scale(glm::vec3(0.5f, 0.5f, 0.5f)));


	//scene 2 - spheres
    scene_2.GetObject(0)->transform.AddTransformation(new Translate(glm::vec3(-2.5f, 0.0f, 0.0f)));
    scene_2.GetObject(1)->transform.AddTransformation(new Translate(glm::vec3(0.0f, -2.5f, 0.0f)));
    scene_2.GetObject(2)->transform.AddTransformation(new Translate(glm::vec3(2.5f, 0.0f, 0.0f)));
    scene_2.GetObject(3)->transform.AddTransformation(new Translate(glm::vec3(0.0f, 2.5f, 0.0f)));


    //scene 4 - solar system
    
    DynamicRotate* dr_sun = new DynamicRotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.01f);


    scene_4.GetObject(0)->transform.AddTransformation(new Rotate(glm::vec3(0.0f, 0.0f, 3.2f)));
    scene_4.GetObject(0)->transform.AddTransformation(new DynamicRotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.008f));

    //earth
    scene_4.GetObject(1)->transform.AddTransformation(new Rotate(glm::vec3(0.0f, 0.0f, 3.2f)));
    scene_4.GetObject(1)->transform.AddTransformation(new Scale(glm::vec3(0.4f, 0.4f, 0.4f)));
    scene_4.GetObject(1)->transform.AddTransformation(new DynamicRotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.005f));  //own rotation
    scene_4.GetObject(1)->transform.AddTransformation(new Translate(glm::vec3(5.0f, 0.0f, 0.0f)));
    scene_4.GetObject(1)->transform.AddTransformation(new DynamicRotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.0025f)); //orbit around sun

    //moon
    scene_4.GetObject(2)->transform.AddTransformation(new Rotate(glm::vec3(0.0f, 0.0f, 3.2f)));
    scene_4.GetObject(2)->transform.AddTransformation(new Scale(glm::vec3(0.1f, 0.1f, 0.1f)));
    scene_4.GetObject(2)->transform.AddTransformation(new Translate(glm::vec3(1.5f, 0.0f, 0.0f))); //position from earth
    scene_4.GetObject(2)->transform.AddTransformation(new DynamicRotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.02f)); //own rotation
    scene_4.GetObject(2)->transform.AddTransformation(new Translate(glm::vec3(5.0f, 0.0f, 0.0f))); //position from sun
    scene_4.GetObject(2)->transform.AddTransformation(new DynamicRotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.0025f)); //orbit around sun and earth


    float lastFrame = 0.0f;
    float deltaTime = 0.0f;
    float f1_pos = 0.0f;

    float aspect = (float)width / (float)height;


    int stencilStartIndex_scene3 = scene_1.GetObjectCount() + scene_2.GetObjectCount();
    int stencilStartIndex_scene5 = scene_1.GetObjectCount() + scene_2.GetObjectCount() + scene_3.GetObjectCount() + scene_4.GetObjectCount();


    camera.UpdateProjection(aspect);

    camera.Attach(&shader_program_1);
	camera.Attach(&shader_program_without_color);
	camera.Attach(&shader_program_light);
	camera.Attach(&shader_program_light_2);
	camera.Attach(&shader_program_light_3);
    camera.Attach(&grass_shader_program);
    camera.Attach(&grass_shader_program_game);
    camera.Attach(&shrek_shader_program);
    camera.Attach(&fiona_shader_program);
    camera.Attach(&f1_shader_program);
    camera.Attach(&f1_shader_program_game);
    camera.Attach(&sun_shader_program);
    camera.Attach(&earth_shader_program);
    camera.Attach(&moon_shader_program);
    camera.Attach(&skybox_shader_program);
    camera.Attach(&shader_program_without_texture);




    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glEnable(GL_DEPTH_TEST);//Do depth comparisons and update the depth buffer.

    //main rendering loop
    while (!glfwWindowShouldClose(window)) 
    {
        // clear color a depth (z-) buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        inputCamera(window, camera, deltaTime);

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        skybox_shader_program.Use();
        skybox_shader_program.SetUniform("view", glm::mat4(glm::mat3(camera.GetViewMatrix())));
        skybox_shader_program.SetUniform("texture1", 7);
        skybox_shader_program.SetUniform("projection", projection);
        cube.Draw();   // skybox

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        if (scene1_initialized)
        {
            scene_1.DrawAll();
            scene_1.GetObject(0)->transform.UpdateTransformation();
        }

        if (scene2_initialized)
        {
            scene_2.DrawAll();           
        }

        if (scene3_initialized)
        {
            if (qKeyPressed)
            {
                DrawableObject* newTree = new DrawableObject(&shader_program_light, &treeModel, true);
                newTree->transform.AddTransformation(new Translate(glm::vec3(worldPos.x, worldPos.y, worldPos.z)));
                scene_3.AddObject(newTree);
                qKeyPressed = false;
            }
            if (leftMousePressed) 
            {
                leftMousePressed = false;
            }
            if (zKeyPressed)
            {
                selectedObjectIndex = getSelectedIndex(&scene_3, stencilStartIndex_scene3);

                if (selectedObjectIndex >= 0 && selectedObjectIndex < scene_3.GetObjectCount())
                {
                    scene_3.RemoveObject(selectedObjectIndex);
                    printf("Selected Object Index to remove: %d\n", selectedObjectIndex);
                    selectedObjectIndex = -1;
                }

                zKeyPressed = false;
            }
            if (arrowUpPressed)
            {
                selectedObjectIndex = getSelectedIndex(&scene_3, stencilStartIndex_scene3);
                if (selectedObjectIndex >= 0 && selectedObjectIndex < scene_3.GetObjectCount())
                {
                    scene_3.GetObject(selectedObjectIndex)->transform.AddTransformation(new Translate(glm::vec3(0.0f, 0.0f, -0.5f)));
                }
                arrowUpPressed = false;
            }
            if (arrowDownPressed)
            {
                selectedObjectIndex = getSelectedIndex(&scene_3, stencilStartIndex_scene3);
                if (selectedObjectIndex >= 0 && selectedObjectIndex < scene_3.GetObjectCount())
                {
                    scene_3.GetObject(selectedObjectIndex)->transform.AddTransformation(new Translate(glm::vec3(0.0f, 0.0f, 0.5f)));
                }
                arrowDownPressed = false;
            }
            if (arrowLeftPressed)
            {
                selectedObjectIndex = getSelectedIndex(&scene_3, stencilStartIndex_scene3);
                if (selectedObjectIndex >= 0 && selectedObjectIndex < scene_3.GetObjectCount())
                {
                    scene_3.GetObject(selectedObjectIndex)->transform.AddTransformation(new Translate(glm::vec3(-0.5f, 0.0f, 0.0f)));
                }
                arrowLeftPressed = false;
            }
            if (arrowRightPressed)
            {
                selectedObjectIndex = getSelectedIndex(&scene_3, stencilStartIndex_scene3);
                if (selectedObjectIndex >= 0 && selectedObjectIndex < scene_3.GetObjectCount())
                {
                    scene_3.GetObject(selectedObjectIndex)->transform.AddTransformation(new Translate(glm::vec3(0.5f, 0.0f, 0.0f)));
                }
                arrowRightPressed = false;
            }
            fireflys[0].transform.UpdateTransformation();


            fireflys[2].transform.UpdateTransformation();
            fireflys[3].transform.UpdateTransformation();
            fireflys[4].transform.UpdateTransformation();

            fireflys_obj[2].transform.UpdateTransformation();
            fireflys_obj[3].transform.UpdateTransformation();
            fireflys_obj[4].transform.UpdateTransformation();

            if (flashlight_on)
            {
                fireflys.back().intensity = 1.0f;
            }
            else
            {
                fireflys.back().intensity = 0.0f;
            }
            fireflys.back().position = camera.GetPosition();
            fireflys.back().direction = camera.GetTargetDirection();
            shader_program_light.Use();
            shader_program_light.SetLight(fireflys);

            grass_shader_program.Use();
            grass_shader_program.SetLight(fireflys);

            shrek_shader_program.Use();
            shrek_shader_program.SetLight(fireflys);
            fiona_shader_program.Use();
            fiona_shader_program.SetLight(fireflys);

            f1_shader_program.Use();
            f1_shader_program.SetLight(fireflys);

            shader_program_without_texture.Use();
            shader_program_without_texture.SetLight(fireflys);

            scene_3.DrawAll();
        }        

        if (scene4_initialized)
        {
            scene_4.DrawAll();

            sun_shader_program.Use();
            sun_shader_program.SetLight(sun_own_light);
            earth_shader_program.Use();
            earth_shader_program.SetLight(lights_solarSystem);
            moon_shader_program.Use();
            moon_shader_program.SetLight(lights_solarSystem);
        }

        if (scene5_initialized)
        {
            scene_5.DrawAll();

            if (leftMousePressed)
            {
                selectedObjectIndex = getSelectedIndex(&scene_5, stencilStartIndex_scene5);

                if (selectedObjectIndex >= 0 && selectedObjectIndex < scene_5.GetObjectCount())
                {
                    auto pos = f1.transform.GetPosition();
                    printf("Selected Object Index: %d\n", selectedObjectIndex);
                    if (selectedObjectIndex != 0)
                    {
                        scene_5.GetObject(selectedObjectIndex)->transform.ClearTransformations();

                        f1.transform.AddTransformation(new Scale(glm::vec3(0.005f)));
                        scene_5.GetObject(selectedObjectIndex)->transform.AddTransformation(
                            new Translate(glm::vec3(20.0f, 0.0f, randFloat(-5.0f, 5.0f)))
                        );
                        scene_5.GetObject(selectedObjectIndex)->transform.AddTransformation(
                            new DynamicTranslate(glm::vec3(0.0f), glm::vec3(-0.05f, 0.0f, 0.0f))
                        );
                    }
                }
                leftMousePressed = false;
            }

            auto pos1 = f1.transform.GetPosition();
            if (fabs(pos1.x + 20.0f) < 0.01f)
            {
                f1.transform.AddTransformation(new Translate(glm::vec3(40.0f, 0.0f, randFloat(-5.0f, 5.0f))));
            }


            grass_shader_program_game.Use();
            f1_shader_program_game.Use();
            f1_shader_program_game.SetLight(lights_game);
            grass_shader_program_game.SetLight(lights_game);
        }
        if (scene6_initialized)
        {
            scene_6.DrawAll();
            if (gKeyPressed)
            {
                bezierSpline.AddPoint(worldPos);
                printf("Added point to Bezier curve: (%f, %f, %f)\n", worldPos.x, worldPos.y, worldPos.z);
                gKeyPressed = false;
            }
            if (hKeyPressed)
            {
                bezierSpline.FinalizeSegments();
                f1_bezier.transform.ClearTransformations();
                f1_bezier.transform.AddTransformation(new Scale(glm::vec3(0.005f)));
                f1_bezier.transform.AddTransformation(&bezierSpline);
                hKeyPressed = false;
            }

            grass_shader_program_game.Use();
            f1_shader_program_game.Use();
            f1_shader_program_game.SetLight(lights_game);
            grass_shader_program_game.SetLight(lights_game);
        }


		//check for errors in shaders
        shader_program_1.CheckCompileError();
        shader_program_without_color.CheckCompileError();
		shader_program_light.CheckCompileError();
		shader_program_light_2.CheckCompileError();
		shader_program_light_3.CheckCompileError();
        grass_shader_program.CheckCompileError();
        shrek_shader_program.CheckCompileError();
        fiona_shader_program.CheckCompileError();
        f1_shader_program.CheckCompileError();
        f1_shader_program_game.CheckCompileError();
        sun_shader_program.CheckCompileError();
        earth_shader_program.CheckCompileError();
        moon_shader_program.CheckCompileError();
        shader_program_without_texture.CheckCompileError();


        // update events
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
