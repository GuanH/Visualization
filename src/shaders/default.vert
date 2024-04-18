#version 450 core
layout(location=0) uniform mat4 modelTrans;
layout(location=1) uniform mat4 viewProjTrans;

layout(location=0) in vec3 vertPos;
layout(location=1) in vec3 vertNorm;

out vec3 fragNorm;

void main(){
    vec4 worldPos = modelTrans * vec4(vertPos, 1.0f);
    gl_Position = viewProjTrans * worldPos;
    fragNorm = vertNorm;
}
