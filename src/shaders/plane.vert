#version 450 core
layout(location=1) uniform mat4 viewProjTrans;
layout(location=3) uniform vec4 p;
out vec2 fragPos;

void main(){
    vec2 tex[4] = {
        vec2(0, 0),
        vec2(0, 1),
        vec2(1, 1),
        vec2(1, 0),
    };
    vec3 pos[3][4] = {
        {
            vec3( p.x,   0,   0),
            vec3( p.x, p.y,   0),
            vec3( p.x, p.y, p.z),
            vec3( p.x,   0, p.z)
        },
        {
            vec3(   0, p.y,   0),
            vec3( p.x, p.y,   0),
            vec3( p.x, p.y, p.z),
            vec3(   0, p.y, p.z)
        },
        {
            vec3(   0,   0, p.z),
            vec3( p.x,   0, p.z),
            vec3( p.x, p.y, p.z),
            vec3(   0, p.y, p.z)
        }
    };
    fragPos = tex[gl_VertexID];
    int ind = int(p.w);
    gl_Position = viewProjTrans * vec4(pos[ind][gl_VertexID], 1.0f);
}
