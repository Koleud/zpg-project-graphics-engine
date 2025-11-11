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
#include "Light.h"
#include "FileUtils.h"
#include "DynamicRotate.h"
#include "DynamicTranslate.h"
#include "Texture.h"

int scene1_initialized = 0;
int scene2_initialized = 0;
int scene3_initialized = 1;
int scene4_initialized = 0;

bool flashlight_on = false;

static void error_callback(int error, const char* description) { fputs(description, stderr); }

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));

bool rightMousePressed = false;
double lastX = 0.0;
double lastY = 0.0;
float yaw = -90.0f;
float pitch = 0.0f;

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
    }
    if (key == GLFW_KEY_2)
    {
        scene1_initialized = 0;
        scene2_initialized = 1;
        scene3_initialized = 0;
        scene4_initialized = 0;
    }
    if (key == GLFW_KEY_3)
    {
        scene1_initialized = 0;
        scene2_initialized = 0;
        scene3_initialized = 1;
        scene4_initialized = 0;
    }
    if (key == GLFW_KEY_4)
    {
        scene1_initialized = 0;
        scene2_initialized = 0;
        scene3_initialized = 0;
        scene4_initialized = 1;
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        flashlight_on = !flashlight_on;
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


    //Lambert's shading model shaders
    const std::string vertex_shader_light = ReadFileToString("Shaders\\lamb.vert");
    const std::string fragment_shader_light = ReadFileToString("Shaders\\lamb.frag");


    //Phong's shading model shaders
    const std::string vertex_shader_light_2 = ReadFileToString("Shaders\\phong.vert");
    const std::string fragment_shader_light_2 = ReadFileToString("Shaders\\phong.frag");


    //Blinn-Phong's shading model shaders
    const std::string vertex_shader_light_3 = ReadFileToString("Shaders\\blinnphong.vert");   //_incorrect
    const std::string fragment_shader_light_3 = ReadFileToString("Shaders\\blinnphong.frag");

    const std::string grass_vertex_shader = ReadFileToString("Shaders\\lamb_grass.vert");
    const std::string grass_fragment_shader = ReadFileToString("Shaders\\lamb_grass.frag");

    const std::string f1_vertex_shader = ReadFileToString("Shaders\\lamb_f1.vert");
    const std::string f1_fragment_shader = ReadFileToString("Shaders\\lamb_f1.frag");

    const std::string shrek_vertex_shader = ReadFileToString("Shaders\\lamb_shrek.vert");
    const std::string shrek_fragment_shader = ReadFileToString("Shaders\\lamb_shrek.frag");
    const std::string fiona_vertex_shader = ReadFileToString("Shaders\\lamb_fiona.vert");
    const std::string fiona_fragment_shader = ReadFileToString("Shaders\\lamb_fiona.frag");


    Texture grassTexture("Textures\\grass.jpg");
    Texture shrekTexture("Models\\shrek\\shrek.png");
    Texture fionaTexture("Models\\shrek\\fiona.png");
    Texture f1Texture("Models\\formula_1\\Substance SpecGloss\\Right ones\\formula1_DefaultMaterial_Diffuse.png");


    // create shaders
    Shader vertex_shader_obj(vertex_shader, GL_VERTEX_SHADER);
    Shader fragment_shader_obj(fragment_shader, GL_FRAGMENT_SHADER);

    Shader vertex_shader_without_color_obj(vertex_shader_without_color, GL_VERTEX_SHADER);
    Shader fragment_shader_without_color_obj(fragment_shader_without_color, GL_FRAGMENT_SHADER);

    Shader vertex_shader_light_obj(vertex_shader_light, GL_VERTEX_SHADER);
    Shader fragment_shader_light_obj(fragment_shader_light, GL_FRAGMENT_SHADER);

	Shader vertex_shader_light_2_obj(vertex_shader_light_2, GL_VERTEX_SHADER);
	Shader fragment_shader_light_2_obj(fragment_shader_light_2, GL_FRAGMENT_SHADER);

    Shader vertex_shader_light_3_obj(vertex_shader_light_3, GL_VERTEX_SHADER);
    Shader fragment_shader_light_3_obj(fragment_shader_light_3, GL_FRAGMENT_SHADER);

    Shader grass_vertex_shader_obj(grass_vertex_shader, GL_VERTEX_SHADER);
    Shader grass_fragment_shader_obj(grass_fragment_shader, GL_FRAGMENT_SHADER);

    Shader f1_vertex_shader_obj(f1_vertex_shader, GL_VERTEX_SHADER);
    Shader f1_fragment_shader_obj(f1_fragment_shader, GL_FRAGMENT_SHADER);

    Shader shrek_vertex_shader_obj(shrek_vertex_shader, GL_VERTEX_SHADER);
    Shader shrek_fragment_shader_obj(shrek_fragment_shader, GL_FRAGMENT_SHADER);
    Shader fiona_vertex_shader_obj(fiona_vertex_shader, GL_VERTEX_SHADER);
    Shader fiona_fragment_shader_obj(fiona_fragment_shader, GL_FRAGMENT_SHADER);



    // create shader program
    ShaderProgram shader_program_1(vertex_shader_obj, fragment_shader_obj, &camera);
    ShaderProgram shader_program_without_color(vertex_shader_without_color_obj, fragment_shader_without_color_obj, &camera);
    ShaderProgram shader_program_light(vertex_shader_light_obj, fragment_shader_light_obj, &camera);
	ShaderProgram shader_program_light_2(vertex_shader_light_2_obj, fragment_shader_light_2_obj, &camera);
	ShaderProgram shader_program_light_3(vertex_shader_light_3_obj, fragment_shader_light_3_obj, &camera);

    ShaderProgram grass_shader_program(grass_vertex_shader_obj, grass_fragment_shader_obj, &camera);

    ShaderProgram f1_shader_program(f1_vertex_shader_obj, f1_fragment_shader_obj, &camera);

    ShaderProgram shrek_shader_program(shrek_vertex_shader_obj, shrek_fragment_shader_obj, &camera);
    ShaderProgram fiona_shader_program(fiona_vertex_shader_obj, fiona_fragment_shader_obj, &camera);


	//create scenes
    Scene scene_1 = Scene();
    Scene scene_2 = Scene();
    Scene scene_3 = Scene();
    Scene scene_4 = Scene();



	// create models
	Model triangleModel(std::vector<float>(points, points + sizeof(points) / sizeof(float)), sizeof(points) / (6 * sizeof(float)));
	Model treeModel(std::vector<float>(tree, tree + sizeof(tree) / sizeof(float)), sizeof(tree) / (6 * sizeof(float)));
	Model bushesModel(std::vector<float>(bushes, bushes + sizeof(bushes) / sizeof(float)), sizeof(bushes) / (6 * sizeof(float)));
	Model plainModel(std::vector<float>(plain, plain + sizeof(plain) / sizeof(float)), sizeof(plain) / (6 * sizeof(float)), true);
	Model sphereModel(std::vector<float>(sphere, sphere + sizeof(sphere) / sizeof(float)), sizeof(sphere) / (6 * sizeof(float)));


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
		DrawableObject tree(&shader_program_light, &treeModel);
        tree.transform.AddTransformation(new Rotate(glm::vec3(0.0f, randFloat(0.0f, 360.0f), 0.0f)));
		tree.transform.AddTransformation(new Translate(glm::vec3(randFloat(-30.0f, 30.0f), 0.0f, randFloat(-30.0f, 30.0f))));
		float scale = randFloat(0.3f, 1.0f);
		tree.transform.AddTransformation(new Scale(glm::vec3(scale, scale, scale)));
		trees.push_back(tree);

		
		DrawableObject bush(&shader_program_light, &bushesModel);
		bush.transform.AddTransformation(new Rotate(glm::vec3(0.0f, randFloat(0.0f, 360.0f), 0.0f)));
		bush.transform.AddTransformation(new Translate(glm::vec3(randFloat(-30.0f, 30.0f), 0.0f, randFloat(-30.0f, 30.0f))));
		float scale_bush = randFloat(0.5f, 1.5f);
		bush.transform.AddTransformation(new Scale(glm::vec3(scale_bush, scale_bush, scale_bush)));
		bushes_list.push_back(bush);
	}

	DrawableObject plain(&grass_shader_program, &plainModel);
	plain.transform.AddTransformation(new Scale(glm::vec3(50.0f, 1.0f, 50.0f)));


    //create Solar system scene objects
    DrawableObject sun(&shader_program_1, &sphereModel);
    DrawableObject earth(&shader_program_1, &sphereModel);
    DrawableObject moon(&shader_program_1, &sphereModel);


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


        // scene 4 - solar system
    scene_4.AddObject(&sun);
    scene_4.AddObject(&earth);
    scene_4.AddObject(&moon);



	// create view and projection matrices
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    //glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 100.0f);
    //glm::mat4 projection = glm::perspective(glm::radians(130.0f), (float)width / (float)height, 0.1f, 100.0f);

	// pass matrices to shader
	shader_program_1.Use();
	shader_program_1.SetUniform("view", view);
	shader_program_1.SetUniform("projection", projection);

    shader_program_without_color.Use();
    shader_program_without_color.SetUniform("view", view);
    shader_program_without_color.SetUniform("projection", projection);


    std::vector<SceneLight> lights;
    SceneLight mainLight(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.385f, 0.647f, 0.812f), 1.0f, 10.0f);

    //mainLight.transform.AddTransformation(new Translate(glm::vec3(0.0f, 5.0f, 0.0f)));

    SceneLight secondaryLight(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.385f, 0.647f, 0.812f), 0.7f, 10.0f);

    lights.emplace_back(mainLight);
    lights.emplace_back(secondaryLight);


    std::vector<SceneLight> fireflys;
    SceneLight mainLight_forest(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.812f, 0.647f, 0.385f), 1.0f, 2.0f);
    SceneLight secondaryLight_forest(glm::vec3(20.0f, 0.5f, 0.0f), glm::vec3(0.812f, 0.647f, 0.385f), 1.0f, 2.0f);
    SceneLight secondaryLight_forest_1(glm::vec3(10.0f, 0.5f, 0.0f), glm::vec3(0.812f, 0.647f, 0.385f), 1.0f, 2.0f);
    SceneLight secondaryLight_forest_2(glm::vec3(15.0f, 0.5f, 0.0f), glm::vec3(0.812f, 0.647f, 0.385f), 1.0f, 2.0f);
    SceneLight secondaryLight_forest_3(glm::vec3(25.0f, 0.5f, 0.0f), glm::vec3(0.812f, 0.647f, 0.385f), 1.0f, 2.0f);
    SceneLight directional_forest(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.3f, 0.3f, 0.3f), 0.5f);
    SceneLight flashlight_forest(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.685f, 0.647f, 0.712f), 1.0f, 21.0f, glm::radians(15.0f));

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


    Model formula_1_model("Models\\formula_1\\Formula_1_mesh.obj", "Models\\formula_1\\Formula_1_mesh.mlt");
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


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

    //earth
    scene_4.GetObject(1)->transform.AddTransformation(new Scale(glm::vec3(0.4f, 0.4f, 0.4f)));
    scene_4.GetObject(1)->transform.AddTransformation(new DynamicRotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.01f));
    scene_4.GetObject(1)->transform.AddTransformation(new Translate(glm::vec3(5.0f, 0.0f, 0.0f)));
    scene_4.GetObject(1)->transform.AddTransformation(new DynamicRotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.005f));

    //moon
    scene_4.GetObject(2)->transform.AddTransformation(new Scale(glm::vec3(0.1f, 0.1f, 0.1f)));
    scene_4.GetObject(2)->transform.AddTransformation(new Translate(glm::vec3(1.5f, 0.0f, 0.0f)));
    scene_4.GetObject(2)->transform.AddTransformation(new DynamicRotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.04f));
    scene_4.GetObject(2)->transform.AddTransformation(new Translate(glm::vec3(5.0f, 0.0f, 0.0f)));
    scene_4.GetObject(2)->transform.AddTransformation(new DynamicRotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.005f));

    /*scene_4.GetObject(1)->transform.UpdateTransformation(1, (new Rotate(glm::vec3(0.0f, angle, 0.0f))));
     scene_4.GetObject(1)->transform.UpdateTransformation(3, (new Rotate(glm::vec3(0.0f, angle, 0.0f))));


     scene_4.GetObject(2)->transform.UpdateTransformation(2, new Rotate(glm::vec3(0.0f, angle_2, 0.0f)));
     scene_4.GetObject(2)->transform.UpdateTransformation(4, new Rotate(glm::vec3(0.0f, angle, 0.0f)));*/



    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    float aspect = (float)width / (float)height;
    camera.UpdateProjection(aspect);

    camera.Attach(&shader_program_1);
	camera.Attach(&shader_program_without_color);
	camera.Attach(&shader_program_light);
	camera.Attach(&shader_program_light_2);
	camera.Attach(&shader_program_light_3);
    camera.Attach(&grass_shader_program);
    camera.Attach(&shrek_shader_program);
    camera.Attach(&fiona_shader_program);
    camera.Attach(&f1_shader_program);


    /*if (glfwSetWindowSizeCallback(window, window_size_callback))
    {

    }*/

    glEnable(GL_DEPTH_TEST);//Do depth comparisons and update the depth buffer.
    // hlavní smyčka
    while (!glfwWindowShouldClose(window)) {
        // clear color a depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        inputCamera(window, camera, deltaTime);

		angle += 0.003f;
        angle_2 += 0.01f;


        if (scene1_initialized)
        {
            scene_1.DrawAll();

			//scene_1.GetObject(0)->transform.UpdateTransformation(0, (new Rotate(glm::vec3(0.0f, 0.0f, angle))));
            scene_1.GetObject(0)->transform.UpdateTransformation(1.0f);
        }

        if (scene2_initialized)
        {
            scene_2.DrawAll();           
        }

        if (scene3_initialized)
        {
            fireflys[0].transform.UpdateTransformation(1.0f);


            fireflys[2].transform.UpdateTransformation(1.0f);
            fireflys[3].transform.UpdateTransformation(1.0f);
            fireflys[4].transform.UpdateTransformation(1.0f);

            fireflys_obj[2].transform.UpdateTransformation(1.0f);
            fireflys_obj[3].transform.UpdateTransformation(1.0f);
            fireflys_obj[4].transform.UpdateTransformation(1.0f);

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



            scene_3.DrawAll();
        }        

        if (scene4_initialized)
        {
            scene_4.DrawAll();

            scene_4.GetObject(1)->transform.UpdateTransformation(1.0f);
            scene_4.GetObject(2)->transform.UpdateTransformation(1.0f);
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

        // update events
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
