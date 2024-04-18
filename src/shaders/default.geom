#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;
in vec3 geomPos[];
out vec3 fragNorm;
void main() {
    fragNorm = normalize(cross(geomPos[1]-geomPos[0], geomPos[2]-geomPos[0]));
    for(int i = 0;i < 3;i++) {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
