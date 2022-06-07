#version 430 core

out vec4 colour_out;

in VS_OUT
{
    vec2 tc;
    float ti;
    vec3 normal;
    vec3 worldPos;
} fs_in;

uniform mat4 model_matrix;
uniform vec3 view_pos;
uniform vec3 background_colour;
uniform int flashlight;

layout(binding=0) uniform sampler2DArray tex;

const vec3 AMBIENT = vec3(0.0, 0.0, 0.0);
const float DIFFUSE = 1.0;
const float SPECULAR = 0.3;

struct lightSource
{
    vec3 pos;
    vec3 colour;
    float reach;
};

vec3 calcLight(lightSource source);

lightSource lights[] = lightSource[](
    lightSource(vec3(0, 5, -3.75), vec3(1, 1, 0.75), 12.0), // Shed light
    lightSource(vec3(50, 100, 30), vec3(1, 1, 1), 40.0)    // Moon
);

vec3 lerp(vec3 a, vec3 b, float interpolant)
{
    return a*(1-interpolant) + b*(interpolant);
}

void main(void){

    // Sample relevant texture
    int texIndex = int(floor(fs_in.ti+0.0001));
    vec3 baseColour = texture(tex, vec3(fs_in.tc, texIndex)).xyz;

    // Lights
    vec3 light = AMBIENT;
    for (int i=0; i<lights.length(); i++)
    {
        light = max( light, calcLight( lights[i] ) );
    }
    if (flashlight==1)
    {
        lightSource fl = lightSource(view_pos, vec3(1, 1, 1), 8.0);
        light = max(light, calcLight(fl));
    }

    vec3 colour = light * baseColour;

    colour_out = vec4(colour, 1);
}

// Lighting - help from https://learnopengl.com/Lighting/Basic-Lighting
vec3 calcLight(lightSource source)
{
    // Diffuse
    vec3 lightDir = normalize(source.pos - fs_in.worldPos);
    vec3 normal = normalize(fs_in.normal);
    float diffuse = max(dot(lightDir, normal), 0.0);

    // Specular
    vec3 viewDir = normalize(view_pos - fs_in.worldPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), 20);

    // Consolidate lighting
    float light = (DIFFUSE*diffuse) + (SPECULAR*specular);

    // Distance
    float dist = length(source.pos-fs_in.worldPos) / source.reach;
    light /= (1+dist);
    return light * source.colour;
}
