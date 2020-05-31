/* Create a simple window using GLFW, draw a triangle
    Author: Meg Coleman
    Updated: May 30, 2020

    Review based on information from https://learnopengl.com/Introduction
*/
//#define GLFW_INCLUDE_GLCOREARB
#include <GL/glew.h> //openGL functions
#include <GLFW/glfw3.h> //window functions
//#include <stdio.h> //fprintf
#include <iostream> //std::cout

//function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(int argc, char** argv)
{
    /* Initialize glfw library */
    if (!glfwInit())
    {
        return -1;
    }

    /* set newer version of openGL (3.3) using window hints */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //needed for mac 


    /* point to GLFWwindow and set parameters*/
    GLFWwindow* myWindow = glfwCreateWindow(320, 320, "My Window Review", NULL, NULL);
    
    /* check for window creation */
    if (myWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* set openGL context --to handle openGL commands */
    glfwMakeContextCurrent(myWindow);

    /* initialize GLEW */
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        //fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        glfwTerminate(); // clean up
        return -1;
    }
    
    /* print versions */
    //fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    std::cout << "Status: Using GLEW: " << glewGetString(GLEW_VERSION) << std::endl;

    //printf("GL version: %s\n", glGetString(GL_VERSION));
    std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;

    std::cout << "GLFW version: " << glfwGetVersionString() <<std::endl;
   
    /* create render window */
    /* for mac, we have to get the frame buffer size or it will not match correctly */
    int width, height; 
    glfwGetFramebufferSize(myWindow, &width, &height);
    glViewport(0, 0, width, height);
    /* register the callback function */
    glfwSetFramebufferSizeCallback(myWindow, framebuffer_size_callback);
    
    /* set colour buffer will be cleared with */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //clear with

    /* set up a triangle --notice counter clockwise -right hand rule */
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    //================================== VAO ==============================================
    /* set up VAO */
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // bind VAO
    glBindVertexArray(VAO); // active, VBO will bind to VAO
    
    //================================= VBO ===========================================
    /* set up VBO */
    unsigned int VBO;
    glGenBuffers(1, &VBO); // identifier value assigned
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind identifier to type of buffer passed in
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pass veritces to buffer

    /* point to the start of the vertex positions data */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // data at location 0
    glEnableVertexAttribArray(0); 

    //============================= VERTEX SHADER =====================================
    /* create a basic vertex shader and compile it */
    // source code
    const char *vertexShaderSource = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "out vec3 color;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "   color = aPos;\n"
        "}";
    // create a vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // pass the source code to shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // compile shader
    glCompileShader(vertexShader);

    /* check compilation successful */
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR: VERTEX SHADER FAILED\n" << infoLog << std::endl;
    }

    //========================= FRAG SHADER ===============================================
    /* create a basic fragment (pixel) shader and compile it */
    const char *fragmentShaderSource =
        "#version 330 core\n" 
        "in vec3 color;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(sin(color) + vec3(0.7), 1.0f);\n"
        "   //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}";

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    /* check compilation successful */
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR: FRAGMENT SHADER FAILED\n" << infoLog << std::endl;
    }

    //=============================== SHADER PROGRAM =====================================
    unsigned int shaderProgram = glCreateProgram();
    /* attach shaders and build program */
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    /* check link success */
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR: PROGRAM NOT LINKED\n" << infoLog << std::endl;
    }

    /* try to run program */
    glUseProgram(shaderProgram);
    /* delete the shader objects, no need any more */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
 

    


    //================================= RENDER ============================================
    /* keep the window open */
    while(!glfwWindowShouldClose(myWindow))
    {
        /* call our function to process input */
        processInput(myWindow);

        /* render commands */
        glClear(GL_COLOR_BUFFER_BIT); //clear back buffer

        glUseProgram(shaderProgram); //now render
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        /* swap back buffer with buffer user sees on screen */
        glfwSwapBuffers(myWindow);
        /* look at queue of events, do something */
        glfwPollEvents();
    }

    glfwTerminate(); // clean up and proper exit
    return 0;

}

/* set viewport to window size */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/* deal with some input */
void processInput(GLFWwindow *window)
{
    /* check key pressed */
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        /* close GLFW */
        glfwSetWindowShouldClose(window, true);
}

