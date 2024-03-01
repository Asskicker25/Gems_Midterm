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
uniform float displaceAmount;
uniform sampler2D sceneTexture;

void main()
{
    float amount = 0.0;

    amount = (1.0 + sin(iTime*6.0)) * 0.5;
	amount *= 1.0 + sin(iTime*16.0) * 0.5;
	amount *= 1.0 + sin(iTime*19.0) * 0.5;
	amount *= 1.0 + sin(iTime*27.0) * 0.5;
	amount = pow(amount, 3);

    amount *= displaceAmount * 0.001;

    vec3 col;

    col.r = texture(sceneTexture, vec2(TexCoords.x + amount, TexCoords.y)).r;
    col.g = texture(sceneTexture, vec2(TexCoords)).g;
    col.b = texture(sceneTexture, vec2(TexCoords.x - amount, TexCoords.y)).b;

    color = vec4(col,1.0);
    return;

    col = texture(sceneTexture, vec2(TexCoords)).rgb;
    float a = col.r;
    color = vec4(a,a,a,1.0);
};
