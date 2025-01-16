#version 330 compatibility

void main()
{
    // This code will execute once for each vertex!
    gl_Position = ftransform();
}
