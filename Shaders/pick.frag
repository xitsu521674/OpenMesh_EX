#version 450

out uint FragColor;

in flat int faceID;

void main()
{
    FragColor = gl_PrimitiveID + 1;
}