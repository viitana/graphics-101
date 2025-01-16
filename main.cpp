
#define STB_IMAGE_IMPLEMENTATION
#define GLAD_GL_IMPLEMENTATION
#define RGFW_IMPLEMENTATION
#define RGFW_USE_XDL

#include <iostream>
#include <math.h> 

#include "glm/glm.hpp" // vector & matrix math library
#include "glm/gtc/type_ptr.hpp"
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

#include "stb/stb_image.h" // image loading library
#include "glad/gl.h" // OpenGL loader
#include "RGFW/RGFW.h" // windowing library

#include "cube.h"
#include "helpers.h"

// Callback for when a key is pressed
void keyfunc(RGFW_window* win, RGFW_Key key, char keyChar, RGFW_keymod keyMod, b8 pressed) {
    // If the user presses the escape key, close the window
    if (key == RGFW_keyEscape && pressed)
    {
        RGFW_window_setShouldClose(win);
    }
}

int main() {
    // Let's create a window!
    RGFW_window* win = RGFW_createWindow(
        "Look mom, it's a window!!",
        RGFW_RECT(0, 0, 1280, 720),
        RGFW_windowNoResize
    );

    // Let's load up OpenGL!
    if (!loadOpenGL(RGFW_getProcAddress))
    {
        std::cout << "Failed to load OpenGL D:" << std::endl;
        return -1;
    }

    // Let's set a function to handle the user pressing a key!
    RGFW_setKeyCallback(keyfunc);

    // glMatrixMode(GL_PROJECTION);
    // glLoadMatrixf(projection);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 10.0f);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view));

    std::string vertexShaderSource, fragmentShaderSource;
    readSourceFile("shaders/vert.glsl", vertexShaderSource);
    readSourceFile("shaders/frag.glsl", fragmentShaderSource);

    GLuint textureID = createAndActivateTexture("res/crate.jpg");

    GLuint vertexShaderID = createShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
    GLuint fragmentShaderID = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());
    
    GLuint shaderProgramID = createShaderProgram(vertexShaderID, fragmentShaderID);

    GLuint vertexBufferID = createVBOWithPositionColorAndTextureCoordAttributes();
    setVBOData(vertexBufferID, sizeof(cubeData), (void*)cubeData);

    glUseProgram(shaderProgramID);

    while (RGFW_window_shouldClose(win) == RGFW_FALSE)
    {
        // Ok, so while the window is open...
        RGFW_window_checkEvents(win, 16);

        model = glm::rotate(model, 0.01f, glm::vec3(0,1,0));

        setUniformMat4(shaderProgramID, "MVP", projection * view * model);

        // glMatrixMode(GL_MODELVIEW);
        // glLoadMatrixf(glm::value_ptr(view));

        // Let's set a background color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glBegin(GL_TRIANGLES);
        // glColor3f(1, 0, 0); glVertex3f(-0.6, -0.75, 0);
        // glColor3f(0, 1, 0); glVertex3f(0.6, -0.75, 0);
        // glColor3f(0, 0, 1); glVertex3f(0, 0.75, 0);
        // glEnd();

        glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeData) / sizeof(float));

        RGFW_window_swapBuffers(win);
    }

    RGFW_window_close(win);
    return 0;
}
