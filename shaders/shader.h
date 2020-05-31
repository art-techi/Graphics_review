/* A basic shader class to make handling easier
    Author: Margaret Coleman
    Updated: May 30, 2020 
    
    Review based on information from https://learnopengl.com/Introduction
*/

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
//#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
    public: 
        unsigned int ID; // program ID instance variable

        Shader(const char* vertexPath, const char* fragmentPath) //build shader in constructor
        {
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;

            vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

            try //try to open files
            {
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;
                // read buffer contents
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                // close 
                vShaderFile.close();
                fShaderFile.close();
                // convert stream to string
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch(std::ifstream::failure e)
            {
                std::cerr << "ERROR FILES NOT READ\n" << std::endl;
            }
            
            // change c++ string to c string
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            // compile vertex shader
            unsigned int vertex, fragment;
            int success;
            char infoLog[512];

            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);

            //check for errors in compilation
            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                std::cerr << "ERROR IN VERTEX SHADER COMPILATION\n" << infoLog << std::endl;
            };

            // compile fragment shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);

            // check for errors 
            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                std::cerr << "ERROR IN FRAGMENT SHADER COMPILATION\n" << infoLog << std::endl;
            }

            // set shader program
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);
            // check errors
            glGetProgramiv(ID, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(ID, 512, NULL, infoLog);
                std::cerr << "ERROR PROGRAM LINKING FAILED\n" << infoLog << std::endl;
            }

            // clean up
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        
        void use() //activate shader
        {
            glUseProgram(ID);
        } 

        void setBool(const std::string &name, bool value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }
        void setInt(const std::string &name, int value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setFloat(const std:: string &name, float value) const
        {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
};

#endif
