#version 430 core

layout (location = 0) in vec3 position;
layout(location = 1) in vec2 tc;
layout(location = 2) in float ti;
layout (location = 3) in vec3 normal;

out VS_OUT
{
    vec2 tc;
    float ti;
    vec3 normal;
    vec3 worldPos;
} vs_out;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

void main(void)
{
    vec4 worldPos = model_matrix * vec4(position, 1.0);
    vec4 viewPos = view_matrix * worldPos;
    vec4 screenPos = proj_matrix * viewPos;
    gl_Position = screenPos;

    vs_out.tc = tc;
    vs_out.ti = ti;
    vs_out.normal = (model_matrix * vec4(normal, 1.0)).xyz;
    vs_out.worldPos = worldPos.xyz;
}
