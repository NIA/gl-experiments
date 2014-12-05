uniform mat4 mvp_matrix;

in vec4 a_position;

void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * a_position;
}
