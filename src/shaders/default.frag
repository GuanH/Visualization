#version 450 core
layout(location=2) uniform vec4 c;
in vec3 fragNorm;

out vec4 color;

void main(){
    vec3 norm = normalize(fragNorm);
    float l1 = dot(norm, normalize(vec3(0.0f, 1.0f, 1.0f)));
    float l2 = dot(norm, normalize(vec3(1.0f, 1.0f, -1.0f)));
    float l3 = dot(norm, normalize(vec3(-1.0f, 1.0f, -1.0f)));
    float str = max(0.3f, max(max(l1, l2), l3));
    color = vec4(c.xyz * str, c.w);
}
