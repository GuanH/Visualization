#version 450 core
layout(location = 0) uniform mat4 invViewProj;
layout(location = 1) uniform vec3 ratio;
out vec3 fragPos1;
out vec3 fragPos2;

void main() {
    const vec4 poss1[] = {
        vec4(-1, -1, 1, 1),
        vec4(1, -1, 1, 1),
        vec4(1, 1, 1, 1),
        vec4(-1, -1, 1, 1),
        vec4(1, 1, 1, 1),
        vec4(-1, 1, 1, 1),
        };
    const vec4 poss2[] = {
        vec4(-1, -1, -1, 1),
        vec4(1, -1, -1, 1),
        vec4(1, 1, -1, 1),
        vec4(-1, -1, -1, 1),
        vec4(1, 1, -1, 1),
        vec4(-1, 1, -1, 1),
        };
    const vec4 pos1 = poss1[gl_VertexID];
    vec4 fp1 = invViewProj * pos1;
    fp1 /= fp1.w;
    fragPos1 = fp1.xyz;
    const vec4 pos2 = poss2[gl_VertexID];
    vec4 fp2 = invViewProj * pos2;
    fp2 /= fp2.w;
    fragPos2 = fp2.xyz;
    gl_Position = vec4(pos1.xyz * ratio, 1.0f);
}
