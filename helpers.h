
#pragma once 

#include <iostream>
#include <fstream>
#include <vector>

#ifndef GLAD_GL_H_
#include <glad/gl.h>
#endif

#ifndef STBI_INCLUDE_STB_IMAGE_H
#include "stb/stb_image.h"
#endif

// Loads OpenGL extension functions using the given loader function pointer.
// This is not particularly relevant for this demo, just library setup boilerplate :)
bool loadOpenGL(void* (*procAddress)(const char*))
{
    return gladLoadGL((GLADloadfunc)procAddress);
}

// Reads the contents of a file into a given output string.
void readSourceFile(const char* path, std::string& out)
{
    // Open the file
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cout << "Unable to open file '" << path << "'" << std::endl;
        return;
    }

    // Read it line by line, adding to result string
    std::string line;
    while (std::getline(file, line))
    {
        out += line + "\n";
    }

    // And finally, close it
    file.close();
}

// Creates a shader based on a given shader type and source code string.
// Returns the ID of the created shader.
GLuint createShader(const int shaderType, const char* vertexShaderSource)
{
    // Create the shader
    GLuint shaderID = glCreateShader(shaderType);
    
    // Set the source code for it
    glShaderSource(shaderID, 1, &vertexShaderSource, nullptr);

    // Compile it
    glCompileShader(shaderID);

    // It there's errors, print them and return 0
    GLint compiled;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE)
    {
        GLint errLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &errLength);

        std::vector<GLchar> errorLog(errLength);
        glGetShaderInfoLog(shaderID, errLength, &errLength, &errorLog[0]);

        for (int i = 0; i < errorLog.size(); i++) {
            std::cout << errorLog[i];
        }
        std::cout << std::endl;
        return 0;
    }

    // If successful, return the shader ID
    return shaderID;
}

// Creates a shader program for a given vertex and fragment shader.
// Returns the ID of the created shader program.
GLuint createShaderProgram(const GLuint vertexShaderID, const GLuint fragmentShaderID)
{
    // Create the shader program
    GLuint shaderProgramID = glCreateProgram();
    
    // Attach the given vertex and fragment shaders to the program
    glAttachShader(shaderProgramID, vertexShaderID);
    glAttachShader(shaderProgramID, fragmentShaderID);

    // Link the program
    glLinkProgram(shaderProgramID);

    // If there's errors, print them and return 0
    GLint programLinked;
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &programLinked);
    if (programLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(shaderProgramID, maxLength, &maxLength, &errorLog[0]);

        for (int i = 0; i < errorLog.size(); i++) {
            std::cout << errorLog[i];
        }
        std::cout << std::endl;
        return 0;
    }

    return shaderProgramID;
}

// Sets a uniform 4x4 matrix value in a given shader program.
void setUniformMat4(const GLuint shaderProgramID, const char* uniformName, const glm::mat4& matrix)
{
    // Get the location ID of the uniform in the given shader program
    GLint uniformLocation = glGetUniformLocation(shaderProgramID, uniformName);
    // Set the uniform value
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}

// Creates a VBO with position, color, and texture coordinate attributes
// and activates said attributes.
// Returns the ID of the created VBO.
GLuint createVBOWithPositionColorAndTextureCoordAttributes()
{
    // Create the VBO
    GLuint vertexBufferID;
    glGenBuffers(1, &vertexBufferID);

    // Bind the VBO as a vertex data buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

    // Vertex positions!
    glVertexAttribPointer(
        0, // 'location' in shader source, e.g. 'layout (location = ?) in vec3 myCoolVector'
        3, // size of the element, e.g. 3 for vec3
        GL_FLOAT, // base type of the element
        GL_FALSE, // should the data be normalized?
        8*sizeof(float), // stride, i.e. how many bytes to skip to get to the next element
        (void*)0 // where to start reading at?
    );

    // Vertex colors!
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        8*sizeof(float),
        (void*)(3*sizeof(float))
    );

    // Vertex texture coordinates!
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        8*sizeof(float),
        (void*)(6*sizeof(float))
    );

    // Enable the attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    return vertexBufferID;
}

// Sets the data for a given VBO.
void setVBOData(const GLuint vboID, const int sizeBytes, const void* data)
{
    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    // Set the data in the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeBytes, data, GL_STATIC_DRAW);
}

// Creates and binds an RGB texture from a given image file path.
GLuint createAndActivateTexture(const char* path)
{
    // Generate a texture "object"
    GLuint textureID;
    glGenTextures(1, &textureID);  

    // Activate and bind the texture object as a 2D texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Load the image data
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data) // If successful, set the image data & parameters
    {
        // Set image format, data & dimensions
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // Set texture wrapping options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        // Set texture filtering/scaling options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        // If there's issues, we let the user know
        std::cout << "Failed to load texture '" << path << "'" << std::endl;
    }

    // Free the image data on the CPU side. It's now uploaded on the GPU :)
    stbi_image_free(data);

    // Return the texture ID
    return textureID;
}