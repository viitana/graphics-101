#define GLAD_GL_IMPLEMENTATION
#define RGFW_IMPLEMENTATION
#define RGFW_USE_XDL
// #define GLEW_NO_GLU
// #define GLEW_STATIC

#include <glad/gl.h>

#include <iostream>
#include <vector>
#include <math.h> 
// #include <GL/glew.h>
#include <RGFW/RGFW.h>

// #include <slim_gl/slim_gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
// #include <GL/glut.h>

// Callback for when a key is pressed
void keyfunc(RGFW_window* win, RGFW_Key key, char keyChar, RGFW_keymod keyMod, b8 pressed) {
    if (key == RGFW_keyEscape && pressed) {
        RGFW_window_setShouldClose(win);
    }
}

const GLfloat cube[12][3][3] = {
    {{-1.0, -1.0, -1.0}, {-1.0, 1.0, -1.0}, {1.0, 1.0, -1.0}},
    {{-1.0, -1.0, -1.0}, {1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}},

    {{1.0, -1.0, -1.0}, {1.0, 1.0, -1.0}, {1.0, 1.0, 1.0}},
    {{1.0, -1.0, -1.0}, {1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}},

    {{1.0, -1.0, 1.0}, {1.0, 1.0, 1.0}, {-1.0, 1.0, 1.0}},
    {{1.0, -1.0, 1.0}, {-1.0, 1.0, 1.0}, {-1.0, -1.0, 1.0}},

    {{-1.0, -1.0, 1.0}, {-1.0, 1.0, 1.0}, {-1.0, 1.0, -1.0}},
    {{-1.0, -1.0, 1.0}, {-1.0, 1.0, -1.0}, {-1.0, -1.0, -1.0}},

    {{-1.0, 1.0, -1.0}, {-1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
    {{-1.0, 1.0, -1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, -1.0}},

    {{1.0, -1.0, 1.0}, {-1.0, -1.0, 1.0}, {-1.0, -1.0, -1.0}},
    {{1.0, -1.0, 1.0}, {-1.0, -1.0, -1.0}, {1.0, -1.0, -1.0}},
};

const GLfloat cubeVertices[3*3*12] = {
    -1.0, -1.0, -1.0, -1.0,  1.0, -1.0,  1.0,  1.0, -1.0,
    -1.0, -1.0, -1.0,  1.0,  1.0, -1.0,  1.0, -1.0, -1.0,

     1.0, -1.0, -1.0,  1.0,  1.0, -1.0,  1.0,  1.0,  1.0,
     1.0, -1.0, -1.0,  1.0,  1.0,  1.0,  1.0, -1.0,  1.0,

     1.0, -1.0,  1.0,  1.0,  1.0,  1.0, -1.0,  1.0,  1.0,
     1.0, -1.0,  1.0, -1.0,  1.0,  1.0, -1.0, -1.0,  1.0,

    -1.0, -1.0,  1.0, -1.0,  1.0,  1.0, -1.0,  1.0, -1.0,
    -1.0, -1.0,  1.0, -1.0,  1.0, -1.0, -1.0, -1.0, -1.0,

    -1.0,  1.0, -1.0, -1.0,  1.0,  1.0,  1.0,  1.0,  1.0,
    -1.0,  1.0, -1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0,

     1.0, -1.0,  1.0, -1.0, -1.0,  1.0, -1.0, -1.0, -1.0,
     1.0, -1.0,  1.0, -1.0, -1.0, -1.0,  1.0, -1.0, -1.0,
};

const GLfloat cubeColors[12][3] = {
    {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0},
    {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
    {1.0, 1.0, 0.0}, {1.0, 1.0, 0.0},
    {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0},
};

const char* vertexShaderSource = R"(
#version 330 compatibility

layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = ftransform();
	// gl_Position = vec4(pos, 1);
}
)";

const char* fragmentShaderSource = R"(
#version 330

in vec3 pos;

void main() {
	gl_FragColor = vec4(1);
}
)";

int main() {
    // GLenum glewStatus = glewInit();
    // if (glewStatus != GLEW_OK)
    // {
    //     std::cout << "Error: " << glewGetErrorString(glewStatus) << std::endl;
    //     return 1;
    // }

    // Let's create a window!
    RGFW_window* win = RGFW_createWindow("Look mom, it's a window :D", RGFW_RECT(0, 0, 1280, 720), RGFW_windowCenter | RGFW_windowNoResize);

    if (!gladLoadGL((GLADloadfunc)RGFW_getProcAddress)) {
        return -1;
    }

    // Let's set a function to handle the situation when the user presses a key!
    RGFW_setKeyCallback(keyfunc);

    // Let's set a projection
    // GLfloat projection[16]; 
    // ComputeFOVProjection(projection, 45.0f, 1280.0f / 720.0f, 0.1f, 100.0f, false);

    // glMatrixMode(GL_PROJECTION);
    // glLoadMatrixf(projection);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view));
    
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
    glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);

    glCompileShader(vertexShaderID);
    glCompileShader(fragmentShaderID);

    GLint vertexCompiled;
    GLint fragmentCompiled;
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &vertexCompiled);
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &fragmentCompiled);

    if (vertexCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(vertexShaderID, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        for (int i = 0; i < errorLog.size(); i++) {
            std::cout << errorLog[i];
        }
        std::cout << std::endl;
        return 1;
    }
    if (fragmentCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fragmentShaderID, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        for (int i = 0; i < errorLog.size(); i++) {
            std::cout << errorLog[i];
        }
        std::cout << std::endl;
        return 1;
    }

    GLuint vertexShaderProgramID = glCreateProgram();
    GLuint fragmentShaderProgramID = glCreateProgram();
    glAttachShader(vertexShaderProgramID, vertexShaderID);
    glAttachShader(fragmentShaderProgramID, fragmentShaderID);

    glLinkProgram(vertexShaderProgramID);
    glLinkProgram(fragmentShaderProgramID);

    GLint vertexProgramLinked;
    GLint fragmentProgramLinked;
    glGetProgramiv(vertexShaderProgramID, GL_LINK_STATUS, &vertexProgramLinked);
    glGetProgramiv(fragmentShaderProgramID, GL_LINK_STATUS, &fragmentProgramLinked);
    
    if (vertexProgramLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(vertexShaderProgramID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(vertexShaderProgramID, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        for (int i = 0; i < errorLog.size(); i++) {
            std::cout << errorLog[i];
        }
        std::cout << std::endl;
        return 1;
    }
    if (fragmentProgramLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(fragmentShaderProgramID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(fragmentShaderProgramID, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        for (int i = 0; i < errorLog.size(); i++) {
            std::cout << errorLog[i];
        }
        std::cout << std::endl;
        return 1;
    }

    GLuint pipelineID;
    glGenProgramPipelines(1, &pipelineID);
    glUseProgramStages(pipelineID, GL_VERTEX_SHADER_BIT, vertexShaderProgramID);
    glUseProgramStages(pipelineID, GL_FRAGMENT_SHADER_BIT, fragmentShaderProgramID);
    
    glBindProgramPipeline(pipelineID);

    // Let's create a vertex buffer
    GLuint vertexBufferID;
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);


    while (RGFW_window_shouldClose(win) == RGFW_FALSE)
    {
        // Ok, so while the window is open...
        RGFW_window_checkEvents(win, 0);

        // view = rotateAbit * view;
        view = glm::rotate(view, 0.01f, glm::vec3(0,1,0));
        // view = view * rotateAbit;
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(glm::value_ptr(view));

        // Let's set a background color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

        // You can use modern OpenGL techniques, but this method is more straightforward for drawing just one triangle.
        glBegin(GL_TRIANGLES);

        for (int i = 0; i < 12; i++)
        {
            glColor3f(cubeColors[i][0], cubeColors[i][1], cubeColors[i][2]);
            for (int j = 0; j < 3; j++)
            {
                glVertex3f(
                    cube[i][j][0],
                    cube[i][j][1],
                    cube[i][j][2]
                );
            }
        }

        // glColor3f(1, 0, 0); glVertex3f(-0.6, -0.75, 0);
        // glColor3f(0, 1, 0); glVertex3f(0.6, -0.75, 0);
        // glColor3f(0, 0, 1); glVertex3f(0, 0.75, 0);
        glEnd();

        RGFW_window_swapBuffers(win);
    }

    RGFW_window_close(win);
    return 0;
}
