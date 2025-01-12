#version 450 core
in vec3 fragPos1;
in vec3 fragPos2;
out vec4 color;

layout(location = 2) uniform sampler3D gradient;
layout(location = 4) uniform sampler1D transfer;
layout(location = 3) uniform vec3 view;

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
    vec3 pos1 = fragPos1 + 0.5f;
    vec3 pos2 = fragPos2 + 0.5f;
    vec3 mn = min(pos1, pos2);
    vec3 mx = max(pos1, pos2);
    if (mx.x > 0 && mn.x < 1 &&
            mx.y > 0 && mn.y < 1 &&
            mx.z > 0 && mn.z < 1) {
        const int STEP_NUM = 128;
        vec3 sstep = (pos1 - pos2) / STEP_NUM;
        vec3 result = vec3(0);
        float k = 1.0f;
        for (int i = 0; i <= STEP_NUM; i++) {
            vec3 pos = pos2 + i * sstep;
            vec4 t = texture(gradient, pos.zyx);
            float val = texture(transfer, t.a).r;
            vec3 norm = -((t.xyz - 0.5f) * 2.0f);
            float l1 = dot(norm, normalize(vec3(0.0f, 1.0f, 1.0f)));
            float l2 = dot(norm, normalize(vec3(1.0f, 1.0f, -1.0f)));
            float l3 = dot(norm, normalize(vec3(-1.0f, 1.0f, -1.0f)));
            float str = max(0.3f, max(max(l1, l2), l3));
            vec3 tem = str * hsv2rgb(vec3(t.a * (240.0f / 360.0f), 1.0f, 1.0f));
            result += k * val * tem;
            k = k * (1 - val);
        }
        color = vec4(result, 1);
        return;
    }
    else discard;
}
