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

int scene1_initialized = 0;
int scene2_initialized = 0;
int scene3_initialized = 1;

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
    }
    if (key == GLFW_KEY_2)
    {
        scene1_initialized = 0;
        scene2_initialized = 1;
        scene3_initialized = 0;
    }
    if (key == GLFW_KEY_3)
    {
        scene1_initialized = 0;
        scene2_initialized = 0;
        scene3_initialized = 1;
    }
}

static void window_focus_callback(GLFWwindow* window, int focused) { printf("window_focus_callback \n"); }

static void window_iconify_callback(GLFWwindow* window, int iconified) { printf("window_iconify_callback \n"); }

static void window_size_callback(GLFWwindow* window, int width, int height) {
	printf("resize %d, %d \n", width, height);
	glViewport(0, 0, width, height);
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


/*

//GLM test

// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 100.0f);

// Camera matrix
glm::mat4 View = glm::lookAt(
    glm::vec3(10, 10, 10), // Camera is at (4,3,-3), in World Space
    glm::vec3(0, 0, 0), // and looks at the origin
    glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
);
// Model matrix : an identity matrix (model will be at the origin)
glm::mat4 Model = glm::mat4(1.0f);

*/

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

    // vrcholy trojúhelníku
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
        -0.15f, -0.10f, 0.0f,  1.0f, 0.0f, 1.0f,   // фиолетовый
         0.15f, -0.10f, 0.0f,  0.0f, 1.0f, 1.0f,   // голубой
         0.15f,  0.10f, 0.0f,  0.0f, 1.0f, 0.5f,   // зеленовато-голубой

        -0.15f, -0.10f, 0.0f,  1.0f, 0.0f, 1.0f,   // фиолетовый
         0.15f,  0.10f, 0.0f,  0.0f, 1.0f, 0.5f,   // зеленовато-голубой
        -0.15f,  0.10f, 0.0f,  1.0f, 0.5f, 0.0f    // оранжево-розовый
    };


    // shadery
    const char* vertex_shader =
        "#version 330\n"
        "layout(location=0) in vec3 vp;"
        "layout(location = 1) in vec3 vc;"
        "uniform mat4 model;"    
        "uniform mat4 view;"        
        "uniform mat4 projection;"
        "out vec3 vertexColor;"
        "void main () {"
        "   gl_Position = projection * view * model * vec4(vp, 1.0);"
      //"   gl_Position = vec4(vp, 1.0);"
        "   vertexColor = vc; "
        "}";

    const char* fragment_shader =
        "#version 330\n"
        "in vec3 vertexColor;"
        "out vec4 fragColor;"
        "void main () {"
        "   fragColor = vec4(vertexColor, 1.0);"
        "}";

    // shadery
    const char* vertex_shader_without_color =
        "#version 330\n"
        "layout(location=0) in vec3 vp;"
        "layout(location = 1) in vec3 vc;"
        "uniform mat4 model;"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        "out vec3 vertexColor;"
        "void main () {"
        "   gl_Position = projection * view * model * vec4(vp, 1.0);"
        "   vertexColor = vec3(0.4, 0.4, 0.4); "
        "}";

    const char* fragment_shader_without_color =
        "#version 330\n"
        "in vec3 vertexColor;"
        "out vec4 fragColor;"
        "void main () {"
        "   fragColor = vec4(0.4, 0.4, 0.4, 1.0);"
        "}";


    const char* vertex_shader_light =
        "#version 330 core\n"
        "layout(location = 0) in vec3 vp;"
        "layout(location = 1) in vec3 vn;" 
        "uniform mat4 model;"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        "out vec4 worldPosition;"
        "out vec3 worldNormal;"
        "void main(void) {"
        "    gl_Position = projection * view * model * vec4(vp, 1.0);"
        "    worldPosition = model * vec4(vp, 1.0);"
        "    worldNormal = mat3(model) * vn;"
        "}";

    const char* fragment_shader_light =
        "#version 330 core\n"
        "in vec4 worldPosition;"
        "in vec3 worldNormal;"
        "out vec4 out_Color;"
        "void main(void) {"
        "    vec3 lightPosition = vec3(0.0, 0.0, 0.0);"
        "    vec3 lightToVector = lightPosition - worldPosition.xyz;"
        "    float dotProduct = max(dot(normalize(lightToVector), normalize(worldNormal)), 0.0);"
        "    vec4 diffuse = dotProduct * vec4(0.385, 0.647, 0.812, 1.0);"   //dotProduct * (color of light)
        "    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);"
        "    out_Color = ambient + diffuse;"
        "}";





    // create shaders
    Shader vertex_shader_obj(vertex_shader, GL_VERTEX_SHADER);
    Shader fragment_shader_obj(fragment_shader, GL_FRAGMENT_SHADER);

    Shader vertex_shader_without_color_obj(vertex_shader_without_color, GL_VERTEX_SHADER);
    Shader fragment_shader_without_color_obj(fragment_shader_without_color, GL_FRAGMENT_SHADER);

    Shader vertex_shader_light_obj(vertex_shader_light, GL_VERTEX_SHADER);
    Shader fragment_shader_light_obj(fragment_shader_light, GL_FRAGMENT_SHADER);


    // create shader program
    ShaderProgram shader_program_1(vertex_shader_obj, fragment_shader_obj, &camera);
    ShaderProgram shader_program_without_color(vertex_shader_without_color_obj, fragment_shader_without_color_obj, &camera);
    ShaderProgram shader_program_light(vertex_shader_light_obj, fragment_shader_light_obj, &camera);


	//create scenes
    Scene scene_1 = Scene();
    Scene scene_2 = Scene();
    Scene scene_3 = Scene();



	// create models
	Model triangleModel(std::vector<float>(points, points + sizeof(points) / sizeof(float)), sizeof(points) / (6 * sizeof(float)));
	Model treeModel(std::vector<float>(tree, tree + sizeof(tree) / sizeof(float)), sizeof(tree) / (6 * sizeof(float)));
	Model bushesModel(std::vector<float>(bushes, bushes + sizeof(bushes) / sizeof(float)), sizeof(bushes) / (6 * sizeof(float)));
	Model plainModel(std::vector<float>(plain, plain + sizeof(plain) / sizeof(float)), sizeof(plain) / (6 * sizeof(float)));
	Model sphereModel(std::vector<float>(sphere, sphere + sizeof(sphere) / sizeof(float)), sizeof(sphere) / (6 * sizeof(float)));


	// create drawable objects
	DrawableObject triangle(&shader_program_1, &triangleModel);


	std::vector<DrawableObject> spheres;
    for (int i = 0; i < 4; i++)
    {
        DrawableObject sphere(&shader_program_light, &sphereModel);
		spheres.push_back(sphere);
    }



	// create multiple trees and bushes with random positions and scales
    std::vector<DrawableObject> trees;
	std::vector<DrawableObject> bushes_list;
	for (int i = 0; i < 50; i++)
	{
		DrawableObject tree(&shader_program_1, &treeModel);
        tree.transform.AddTransformation(new Rotate(glm::vec3(0.0f, randFloat(0.0f, 360.0f), 0.0f)));
		tree.transform.AddTransformation(new Translate(glm::vec3(randFloat(-30.0f, 30.0f), 0.0f, randFloat(-30.0f, 30.0f))));
		float scale = randFloat(0.3f, 1.0f);
		tree.transform.AddTransformation(new Scale(glm::vec3(scale, scale, scale)));
		trees.push_back(tree);

		
		DrawableObject bush(&shader_program_1, &bushesModel);
		bush.transform.AddTransformation(new Rotate(glm::vec3(0.0f, randFloat(0.0f, 360.0f), 0.0f)));
		bush.transform.AddTransformation(new Translate(glm::vec3(randFloat(-30.0f, 30.0f), 0.0f, randFloat(-30.0f, 30.0f))));
		float scale_bush = randFloat(0.5f, 1.5f);
		bush.transform.AddTransformation(new Scale(glm::vec3(scale_bush, scale_bush, scale_bush)));
		bushes_list.push_back(bush);
	}

	DrawableObject plain(&shader_program_1, &plainModel);
	plain.transform.AddTransformation(new Scale(glm::vec3(50.0f, 1.0f, 50.0f)));



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



	// create view and projection matrices
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	// pass matrices to shader
	shader_program_1.Use();
	shader_program_1.SetUniform("view", view);
	shader_program_1.SetUniform("projection", projection);

    shader_program_without_color.Use();
    shader_program_without_color.SetUniform("view", view);
    shader_program_without_color.SetUniform("projection", projection);

    shader_program_light.Use();
    shader_program_light.SetUniform("view", view);
    shader_program_light.SetUniform("projection", projection);





	// set clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glClearColor(0.2f, 0.4f, 0.7f, 1.0f);

	float angle = 0.0f;

	//some pre-transofrmations
 
        //scene 1
    scene_1.GetObject(0)->transform.AddTransformation(new Translate(glm::vec3(0.5f, 0.5f, 0.0f)));
    scene_1.GetObject(0)->transform.AddTransformation(new Rotate(glm::vec3(0.0f, 0.0f, angle)));
    scene_1.GetObject(0)->transform.AddTransformation(new Scale(glm::vec3(0.5f, 0.5f, 0.5f)));

	    //scene 2 - spheres
    scene_2.GetObject(0)->transform.AddTransformation(new Translate(glm::vec3(-2.5f, 0.0f, 0.0f)));
    scene_2.GetObject(1)->transform.AddTransformation(new Translate(glm::vec3(0.0f, -2.5f, 0.0f)));
    scene_2.GetObject(2)->transform.AddTransformation(new Translate(glm::vec3(2.5f, 0.0f, 0.0f)));
    scene_2.GetObject(3)->transform.AddTransformation(new Translate(glm::vec3(0.0f, 2.5f, 0.0f)));



    float lastFrame = 0.0f;
    float deltaTime = 0.0f;
    
    camera.Attach(&shader_program_1);
	camera.Attach(&shader_program_without_color);
	camera.Attach(&shader_program_light);

    glEnable(GL_DEPTH_TEST);//Do depth comparisons and update the depth buffer.
    // hlavní smyčka
    while (!glfwWindowShouldClose(window)) {
        // clear color a depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        inputCamera(window, camera, deltaTime);

		angle += 0.01f;


        if (scene1_initialized)
        {
            scene_1.DrawAll();

			scene_1.GetObject(0)->transform.UpdateTransformation(0, (new Rotate(glm::vec3(0.0f, 0.0f, angle))));
        }

        if (scene2_initialized)
        {
            scene_2.DrawAll();
        }

        if (scene3_initialized)
        {
            scene_3.DrawAll();
        }


		//check for errors in shaders
        shader_program_1.CheckCompileError();
        shader_program_without_color.CheckCompileError();
		shader_program_light.CheckCompileError();

        // update events
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
