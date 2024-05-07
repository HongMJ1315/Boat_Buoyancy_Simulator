//Vertex shader for Phong shading + texture mapping
varying vec3 N; //normal vector
varying vec3 E; //V vector for specular.
varying vec3 FP; //fragment position
varying vec3 L; // Light direction
uniform float t;

void main()
{
   vec4 eyePosition = gl_ModelViewMatrix*gl_Vertex;
   vec4 eyeLightPos = gl_ModelViewMatrix*gl_LightSource[0].position;

   N =  normalize(gl_NormalMatrix*gl_Normal);
   // E = -eyePosition.xyz;
   E = -eyePosition.xyz;
//    L = normalize(eyeLightPos.xyz - eyePosition.xyz); // Compute light direction
   L = normalize(gl_LightSource[0].position.xyz); // Compute light direction

   gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
   FP = vec3(gl_ModelViewMatrix*gl_Vertex);
}