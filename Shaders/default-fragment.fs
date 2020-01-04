#version 320 es
out mediump vec4 FragColor;
in mediump vec3 COLOR;

void main()
{
    FragColor = vec4(COLOR, 1.0);
}