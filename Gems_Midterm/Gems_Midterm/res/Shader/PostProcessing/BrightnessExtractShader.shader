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

uniform sampler2D sceneTexture;

void main()
{
	vec4 texColor = texture(sceneTexture, TexCoords);

    float brightness = dot(texColor.rgb, vec3(0.2126, 0.7152, 0.0722));

    if(brightness > 1.0)
        color = vec4(texColor.rgb, 1.0);
	else
		color = vec4(0.0, 0.0, 0.0, 1.0);
};