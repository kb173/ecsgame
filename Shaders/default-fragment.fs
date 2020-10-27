#version 320 es
out mediump vec4 FragColor;

in mediump vec2 TexCoord;
in mediump vec3 Normal;
in mediump vec3 FragPos;
in mediump vec4 FragPosLightSpace;

layout(binding=0) uniform sampler2D shadowMap;
layout(binding=1) uniform sampler2D tex;

uniform mediump vec3 lightDirection;
uniform mediump vec3 cameraPosition;

uniform mediump float diffuseStrength;
uniform mediump float specularStrength;

mediump float ShadowCalculation(vec4 fragPosLightSpace)
{
    // The bias varies depending on the angle to the light (the steeper the angle, the bigger the bias needs to be)
    mediump float bias = max(0.005 * (1.0 - dot(Normal, lightDirection)), 0.0005);

    // perform perspective divide
    mediump vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // If we're outside of [0.0, 1.0] in the coordinates, return 0
    if (projCoords.x < 0.0 || projCoords.y < 0.0 || projCoords.x > 1.0 || projCoords.y > 1.0) return 0.0;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    mediump float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    mediump float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    mediump float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}  


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

    // Shadow
    mediump float shadow = ShadowCalculation(FragPosLightSpace); 

    // Total
    mediump float light = min(diff * diffuseStrength + ambient + spec * specularStrength - shadow * 0.5 + 0.5, 2.0);

    // Assign resulting color
    mediump vec3 color = texColor.xyz * light;
    FragColor = vec4(color, texColor.a);
}