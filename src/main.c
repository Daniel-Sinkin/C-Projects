#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mach/vm_region.h>

GLFWwindow* g_window;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";


void framebufferSizeCallback(
    GLFWwindow* window __attribute__((unused)),
    const int width,
    const int height)
{
    glViewport(0, 0, width, height);
}

void processInput() {
    if(glfwGetKey(g_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(g_window, true);
}

int main() {
    printf("Initializing OpenGL.\n");
    if(!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW!\n");
        return EXIT_FAILURE;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    g_window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (g_window == NULL) {
        fprintf(stderr, "Failed to create GLFW window");
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(g_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD");
        return EXIT_FAILURE;
    }

    int frame_buffer_width, frame_buffer_height;
    // this is in general different from the window size, for example on Apple retina displays which downscale your window.
    glfwGetFramebufferSize(g_window, &frame_buffer_width, &frame_buffer_height);
    glViewport(0, 0, frame_buffer_width, frame_buffer_height);

    glfwSetFramebufferSizeCallback(g_window, framebufferSizeCallback);

    const float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f,  // bottom left
       -0.5f,  0.5f, 0.0f,  // top left
        0.0f,  0.5f, 0.0f   // top center
   };
    const unsigned int indices[] = {
        3, 2, 0,
        2, 1, 0
    };

    unsigned int VAO, VBO, EBO;

    // Generate and bind the VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind the VBO for the vertex data
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Generate and bind the EBO for the element (index) data
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO (optionally)
    glBindVertexArray(0);

    printf("Trying to compile and link shaders into a shader program.");
    int success_flag;
    const unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success_flag);
    if(!success_flag) {
        char infoLog[512];
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
        fprintf(stderr, infoLog);
        return EXIT_FAILURE;
    }

    const unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success_flag);
    if(!success_flag) {
        char info_log[512];
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
        fprintf(stderr, info_log);
        return EXIT_FAILURE;
    }

    printf("Finished compiling shaders, creating shader program now.");
    const unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success_flag);
    if(!success_flag) {
        char info_log[512];
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
        fprintf(stderr, info_log);
    }

    printf("Shaders successfully compiled!\n");

    const GLubyte* version = glGetString(GL_VERSION);
    printf("Machine has OpenGL version: '%s'\n", (const char*)version);
    printf("Minimal required OpenGL version is 3.3.0.\n");

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    printf("\n");
    printf("Finished initializing OpenGL.\n");
    printf("Starting mainloopL.\n");
    while(!glfwWindowShouldClose(g_window)) {
        processInput(g_window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }
    printf("Finished running mainloop.\n");

    glfwTerminate();
    return EXIT_SUCCESS;
}