#ifndef GLSL_H
#define GLSL_H
/*------------------------------------------------------------------------------------------
 * Procedure to set up the vertex and fragment shader.
 * This proce4dure also create a program object containing the shader.
 */
void setGLSLshaders(char *VertexShaderFileName, char *FragmentShaderFileName);

/*---------------------------------------------------------------------------------------------
 * Return the program object handle (ID).
 */
unsigned int ReturnProgramID();
#endif