#version 330

in vec3 color;
in vec2 texCoord;

uniform sampler2D texture1;

void main() {
    gl_FragColor = texture(texture1, texCoord);
}
