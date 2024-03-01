#vertex

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

out vec2 TexCoords;

void main()
{
	TexCoords = texCoords;
    gl_Position = vec4(position,1.0);
};

#fragment

#version 330 core

out vec4 color;
in vec2 TexCoords;

uniform float iTime;
uniform sampler2D sceneTexture;
uniform float amount;

float noise(float x)
{
    return     sin(x * 100.0) * .1 +
            sin((x * 200.0) + 3.0) * 0.05 +
            fract(sin((x * 19.0) + 1.0) * 33.33) * 0.13;
}

void main()
{
    vec2 p_m = TexCoords;
    vec2 p_d = p_m;

    p_d.xy -= iTime * 0.1;

    vec2 dst_map_val = vec2(noise(p_d.y), noise(p_d.x));

    vec2 dst_offset = dst_map_val.xy;

    dst_offset -= vec2(0.5, 0.5);
    dst_offset *= 2;
    dst_offset *= 0.01;

    dst_offset *= (1.0 - p_m.t);
    dst_offset *= amount;

    vec2 dist_tex_coord = p_m.st + dst_offset;
    
    color = texture(sceneTexture,dist_tex_coord);
};
