#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_MVP;

out vec2 v_TexCoord;

void main()
{
    v_TexCoord = texCoord;
    gl_Position = u_MVP * vec4(position, 0.0, 1.0);
}

#shader fragment
#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

out vec4 color;

void main()
{
    vec4 tex = texture(u_Texture, v_TexCoord);
    color = tex * u_Color;
}
