#version 450 core
layout(location=2) uniform sampler2D tex;
layout(location=4) uniform float opaque;
in vec2 fragPos;
out vec4 color;

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main(){
    color=vec4(hsv2rgb(vec3(texture(tex, fragPos).r*(240.0f/360.0f), 1.0f, 1.0f)), opaque);
}
