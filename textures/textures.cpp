/* Put some textures on our rectangle
    Author: Meg Coleman
    Updated: May 31, 2020
    
    Review based on information from https://learnopengl.com/Introduction
*/
//#define GLFW_INCLUDE_GLCOREARB
#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h> //openGL functions
#include <GLFW/glfw3.h> //window functions
#include "shader.h" 
#include "stb_image.h"
//#include <stdio.h> //fprintf
#include <iostream> //std::cout
#include <cmath>

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

    // check number of vertext attributes supported by hardware 
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
    
    /* create render window */
    /* for mac, we have to get the frame buffer size or it will not match correctly */
    int width, height; 
    glfwGetFramebufferSize(myWindow, &width, &height);
    glViewport(0, 0, width, height);
    /* register the callback function */
    glfwSetFramebufferSizeCallback(myWindow, framebuffer_size_callback);
    
    /* set colour buffer will be cleared with */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //clear with

    /* set up a rectangle */
    float vertices[] = {
        /* there are four vertices due to overlap 
           essentially two triangles to make the one rectangle */
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,  // top right
        0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, // top left
    };
    /* indices for ebo */
    unsigned int indices[] = {
        0, 1, 3, // triangle 1
        1, 2, 3 // triangle 2
    };

    float texCoords[] = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f    
    };

    //================================== VAO ==============================================
    /* set up VAO */
    unsigned int VAO;
    glGenVertexArrays(1, &VAO); //generate VAO names
    glBindVertexArray(VAO); // active, VBO will bind to VAO
    
    //================================= VBO ===========================================
    /* set up VBO */
    unsigned int VBO;
    glGenBuffers(1, &VBO); // identifier value assigned
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind identifier to type of buffer passed in
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pass veritces to buffer

    /* point to the start of the vertex positions attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // data at location 0
    glEnableVertexAttribArray(0); 

    /* add a pointer for the colour attribute */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    //=================================== EBO =========================================
    /* set up EBO -Element Buffer Object */
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //=================================== VBO TEX COORDS ===============================
    unsigned int texVBO;
    glGenBuffers(1, &texVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

    /* add pointer for texture attribute */
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

    /* now load the shader defined in shader.h */
    Shader myShader("textures/shader.vs", "textures/shader.fs");

    /* generate and bind texture */
    unsigned int texture; // texture ID
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE1); 
    glBindTexture(GL_TEXTURE_2D, texture);

    /* set wraping options */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* retrieve texture from file */
    int texWidth, texHeight, nrChannels;
    unsigned char *data = stbi_load("textures/woodtexture.jpeg", &texWidth, &texHeight, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
        std::cerr << "FAILED TO LOAD TEXTURE " << std::endl;

    }
    
    glBindTexture(GL_TEXTURE_2D, 0); // unbind 
    stbi_image_free(data); // free up memory
    
    /* wireframe mode set */
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //================================= RENDER ============================================
    /* keep the window open */
    while(!glfwWindowShouldClose(myWindow))
    {
        /* call our function to process input */
        processInput(myWindow);

        /* render commands */
        glClear(GL_COLOR_BUFFER_BIT); //clear back buffer

        myShader.use(); 
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        glActiveTexture(GL_TEXTURE1);
        //float timeValue = glfwGetTime();
        //float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(myShader.ID, "myTexture");
        //glUniform4f(vertexColorLocation, greenValue, greenValue, greenValue, 1.0f);
        glUniform1i(vertexColorLocation, 1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        /* unbind objects */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        
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