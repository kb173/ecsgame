#version 320 es
out mediump vec4 FragColor;

in mediump vec2 TexCoord;
in mediump vec3 Normal;
in mediump vec3 FragPos;

uniform sampler2D tex;
uniform mediump vec3 lightDirection;
uniform mediump vec3 cameraPosition;

uniform mediump float diffuseStrength;
uniform mediump float specularStrength;

void main()
{
    mediump vec4 texColor = texture(tex, TexCoord);

    // Alpha Scissors
    if(texColor.a < 0.1)
        discard;

    // Lighting
    // Diffuse
    mediump float diff = max(dot(Normal, lightDirection), 0.0);

    // Ambient
    mediump float ambient = 0.2;

    // Specular
    mediump vec3 viewDir = normalize(cameraPosition - FragPos);
    mediump vec3 reflectDir = reflect(-lightDirection, Normal);

    mediump float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    // Total
    mediump float light = min(diff * diffuseStrength + ambient + spec * specularStrength, 1.0);

    // Assign resulting color
    mediump vec3 color = texColor.xyz * light;
    FragColor = vec4(color, texColor.a);
}