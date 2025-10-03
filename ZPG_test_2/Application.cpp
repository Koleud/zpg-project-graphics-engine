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

static void error_callback(int error, const char* description) { fputs(description, stderr); }

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
}

static void window_focus_callback(GLFWwindow* window, int focused) { printf("window_focus_callback \n"); }

static void window_iconify_callback(GLFWwindow* window, int iconified) { printf("window_iconify_callback \n"); }

static void window_size_callback(GLFWwindow* window, int width, int height) {
	printf("resize %d, %d \n", width, height);
	glViewport(0, 0, width, height);
}

static void cursor_callback(GLFWwindow* window, double x, double y) { printf("cursor_callback \n"); }

static void button_callback(GLFWwindow* window, int button, int action, int mode) {
	if (action == GLFW_PRESS) printf("button_callback [%d,%d,%d]\n", button, action, mode);
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
    0.6f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.9f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.9f, 0.7f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.6f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.9f, 0.7f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.6f, 0.7f, 0.0f, 1.0f, 1.0f, 0.0f
    };

    // shadery
    const char* vertex_shader =
        "#version 330\n"
        "layout(location=0) in vec3 vp;"
        "layout(location = 1) in vec3 vc;"
        "out vec3 vertexColor;"
        "void main () {"
        "   gl_Position = vec4(vp, 1.0);"
        "   vertexColor = vc; "
        "}";

    const char* fragment_shader =
        "#version 330\n"
        "in vec3 vertexColor;"
        "out vec4 fragColor;"
        "void main () {"
        "   fragColor = vec4(vertexColor, 1.0);"
        "}";


    // create shaders
    Shader vertex_shader_obj(vertex_shader, GL_VERTEX_SHADER);
    Shader fragment_shader_obj(fragment_shader, GL_FRAGMENT_SHADER);

    // create shader program
    ShaderProgram shader_program_1(std::vector<float>(points, points + sizeof(points) / sizeof(float)), vertex_shader_obj, fragment_shader_obj, 3);
    ShaderProgram shader_program_2(std::vector<float>(points_с, points_с + sizeof(points_с) / sizeof(float)), vertex_shader_obj, fragment_shader_obj, 6);


    // hlavní smyčka
    while (!glfwWindowShouldClose(window)) {
        // clear color a depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_program_1.Use();
        shader_program_1.Draw();

        shader_program_2.Use();
        shader_program_2.Draw();

        shader_program_1.CheckCompileError();
        shader_program_2.CheckCompileError();

        // update events
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
