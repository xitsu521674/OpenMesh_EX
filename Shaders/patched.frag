#version 450

in vec2 tex;
out vec4 FragColor;
uniform sampler2D texture1;

void main()
{
    vec3 col = texture(texture1,tex).rgb;
    FragColor = vec4(col,1.0);
    //FragColor = vec4(1,0,0,1);
}