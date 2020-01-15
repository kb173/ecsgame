#version 320 es
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 NORMAL;
layout (location = 2) in vec2 UV;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = UV;
    Normal = NORMAL;
    FragPos = vec3(model * vec4(aPos, 1.0));
}