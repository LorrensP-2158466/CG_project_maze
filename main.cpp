
#include <glm/vec3.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    std::cout << "window resize\n";
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

uint32_t compile_vert_shader_from_file(const char* file_name){
    std::ifstream t(file_name);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    auto shader_source = str.c_str();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shader_source, nullptr);
    glCompileShader(vertexShader);

    int success;
    char info_log[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    }
    return vertexShader;
}

uint32_t compile_frag_shader_from_file(const char* file_name){
    std::ifstream t(file_name);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    auto shader_source = str.c_str();
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shader_source, nullptr);
    glCompileShader(fragmentShader);
    int success;
    char info_log[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    }
    return fragmentShader;
}

int main() {
     if (!glfwInit()){
         return -1;
     }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::vector<glm::vec3> vertices= {
            {-0.5f, -0.5f, 0.0f},
            {0.5f, -0.5f, 0.0f},
            {0.0f, 0.5f, 0.0f}
    };


    unsigned int vertex_shader = compile_vert_shader_from_file("shader.vert");
    unsigned int frag_shader = compile_frag_shader_from_file("shader.frag");
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex_shader);
    glAttachShader(shaderProgram, frag_shader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);


    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, glm::vec3::length(),GL_FLOAT, GL_FALSE,  3 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);




    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);
    glfwTerminate();
    return 0;
}