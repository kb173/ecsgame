#version 320 es
out mediump vec4 FragColor;

in mediump vec2 TexCoord;
in mediump vec3 Normal;

uniform sampler2D tex;

void main()
{
    mediump vec4 texColor = texture(tex, TexCoord);

    // Alpha Scissors
    if(texColor.a < 0.1)
        discard;

    FragColor = texColor;
}