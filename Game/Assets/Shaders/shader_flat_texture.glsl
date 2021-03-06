//-VERT-SHADER-

#version 150

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec2 Texcoord;

out vec3 Position_worldspace;
out vec3 LightPosition_worldspace;

out vec3 Normal_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 EyeDirection_cameraspace;

//TODO do MVP multiplcation on client side?
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 lightPos;

void main()
{
    Texcoord = texcoord;
    
    gl_Position = proj*view*model*vec4(position, 1.0);

    LightPosition_worldspace = lightPos;

    // Position of vertex in worldspace
    Position_worldspace = (model * vec4(position,1)).xyz;

    // Vector from vertex to camera, in camera space
    vec3 Position_cameraspace = (view * model * vec4(position,1)).xyz;
    EyeDirection_cameraspace = vec3(0,0,0) - Position_cameraspace;

    // Vector from vertex to light, in camera space
    vec3 LightPosition_cameraspace = (view * vec4(lightPos,1)).xyz;
    LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

    // Normal of vertex in camera space
    Normal_cameraspace = ( view * model * vec4(normal,0)).xyz; // TODO Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
}

//-FRAG-SHADER-

#version 150

in vec2 Texcoord;

in vec3 Position_worldspace;
in vec3 LightPosition_worldspace;

in vec3 Normal_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 EyeDirection_cameraspace;

out vec4 outColor;

uniform vec3 lightColor;
uniform float lightPower;

uniform vec3 colorDiffuse;      // [SERIALIZABLE]
uniform vec3 colorAmbient;      // [SERIALIZABLE]
uniform sampler2D tex;          // [SERIALIZABLE]

void main() 
{
    // material colors
    vec4 MaterialDiffuseColor	= vec4(colorDiffuse, 1.0);
    vec4 MaterialAmbientColor	= vec4(colorAmbient, 1.0) * MaterialDiffuseColor;

    // cosine of angle between normal and light direction, for diffuse lighting:
    vec3 n = normalize(Normal_cameraspace);
    vec3 l = normalize(LightDirection_cameraspace);
    float cosTheta = clamp(dot(n, l), 0,1);

    // distance to the light
    float distanceSqrd = pow(distance(Position_worldspace, LightPosition_worldspace),2);

    // combine all lighting types for final color
    vec4 tempoutColor =	MaterialAmbientColor + 
                lightPower * MaterialDiffuseColor * vec4(lightColor,1.0) * cosTheta / distanceSqrd;

    outColor = texture(tex, Texcoord)*tempoutColor;
}