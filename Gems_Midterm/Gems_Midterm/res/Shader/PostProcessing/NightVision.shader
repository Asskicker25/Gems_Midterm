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


float hash(in float n) { return fract(sin(n)*43758.5453123); }

void main()
{
    vec3 c = texture(sceneTexture, TexCoords).xyz;

    c += sin(hash(iTime) * 0.01f);
    c += hash((hash(TexCoords.x) + TexCoords.y) * iTime) * 0.5;
    c *= smoothstep(length(TexCoords * TexCoords * TexCoords * vec2(0.075, 0.4)), 1.0, 0.4);
    c *= smoothstep(0.001, 3.5, iTime) * 1.5;

    c = dot(c, vec3(0.2126, 0.7152, 0.0722)) * vec3(0.2, 1.5 - hash(iTime) * 0.1, 0.4);

    color = vec4(c, 1.0);
};
