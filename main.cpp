
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


    while (RGFW_window_shouldClose(win) == RGFW_FALSE)
    {
        // Ok, so while the window is open...
        RGFW_window_checkEvents(win, 16);

        // Let's set a background color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // We should draw something here...

        // Prepares the window for the next draw cycle :)
        RGFW_window_swapBuffers(win);
    }

    RGFW_window_close(win);
    return 0;
}
