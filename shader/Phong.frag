//Fragment shader for Phong shading + texture mapping
varying vec3 N;
varying vec3 E;
varying vec3 FP;
varying vec3 L; // Light direction
uniform sampler2D myTex;

void main()  {
   vec3  Normal = normalize(N);
   vec3  Eye = normalize(E);
   vec4  texel, specular, ambient, diffuse;
   vec4  matSpecular;

   matSpecular = vec4(1.0, 1.0, 1.0, 1.0);
   texel = texture2D(myTex, gl_TexCoord[0].st);
   vec3 viewDir = normalize(Eye - FP); // Corrected view direction
   
   vec3 lightDir = normalize(L); // Use passed light direction
   float diff = max(dot(Normal, lightDir), 0.0);
   vec3 reflectDir = reflect(-lightDir, Normal); // Corrected reflection direction
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), gl_FrontMaterial.shininess);

   diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * texel * diff;
   specular = gl_FrontMaterial.specular * gl_LightSource[0].specular * matSpecular * spec;
   ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient * texel;
	
   gl_FragColor = ambient + diffuse + specular ; 
}
