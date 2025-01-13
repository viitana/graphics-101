#version 330 compatibility

layout (location = 0) in vec3 positionAttrib;
layout (location = 1) in vec3 colorAttrib;
layout (location = 2) in vec2 texCoordAttrib;

uniform mat4 MVP;

out vec3 color;
out vec2 texCoord;

void main()
{
    vec4 position = vec4(positionAttrib, 1);
    vec4 transformed = MVP * position;

    gl_Position = transformed;

    color = colorAttrib;
    texCoord = texCoordAttrib;

    // gl_Position = ftransform();
	// gl_Position = vec4(positionAttrib, 1);
}
