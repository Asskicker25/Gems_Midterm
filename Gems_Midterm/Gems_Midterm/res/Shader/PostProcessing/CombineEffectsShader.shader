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

uniform bool isEffectEnabled;

uniform sampler2D sceneTexture;
uniform sampler2D effectTexture;

void main()
{
    const float gamma = 2.2;

	vec3 hdrColor = texture(sceneTexture, TexCoords).rgb;
    vec3 effectColor = texture(effectTexture, TexCoords).rgb;

    if(isEffectEnabled)
    {
        hdrColor += effectColor;
    }

    color = vec4(hdrColor,1.0);
};