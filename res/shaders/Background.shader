#shader vertex
#version 330 core

layout(location = 0) in vec2 position;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;

void main()
{
    // normalize Y coordinate: 0 at bottom, 1 at top
    float y = gl_FragCoord.y / 480.0;

    vec3 topColor    = vec3(0.2, 0.25, 0.3);
    vec3 bottomColor = vec3(0.05, 0.05, 0.05);

    vec3 finalColor  = mix(bottomColor, topColor, y);
    FragColor = vec4(finalColor, 1.0);
}
