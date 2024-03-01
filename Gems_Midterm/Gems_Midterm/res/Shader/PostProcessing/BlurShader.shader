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

layout(location = 0) out vec4 FragColor1;
layout(location = 1) out vec4 FragColor2;

in vec2 TexCoords;

uniform sampler2D image;

uniform bool horizontal;
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{             
     vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
     vec3 result = texture(image, TexCoords).rgb * weight[0];


     if(horizontal)
     {
         for(int i = 1; i < 5; ++i)
         {
            result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb *weight[i] ;
            result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb *weight[i] ;
         }

        FragColor2 = vec4(result, 1.0);
     }
     else
     {
         for(int i = 1; i < 5; ++i)
         {
             result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i] ;
             result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i] ;
         }

         FragColor1 = vec4(result, 1.0);
     }
}