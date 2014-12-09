#version 150

// Per vertex lighting - vertex shader. Outputs a varying (intepolated) color to the 
// fragment shader
smooth out vec4 color;

// Incoming vertex and normal attributes
in vec3 vertexPosition;		// Vertex position attribute
in vec3 vertexNormal;			// Vertex normal attribute

// Uniforms for material properties
uniform vec4   materialAmbient;
uniform vec4   materialDiffuse;
uniform vec4   materialSpecular;
uniform vec4   materialEmission;
uniform float  materialShininess;

// Global lighting environment ambient intensity
uniform vec4  globalLightAmbient;

// Global camera position
uniform vec3  cameraPosition;

// Uniforms for matrices
uniform mat4 pvm;		// Composite projection, view, model matrix
uniform mat4 modelMatrix;	// Modeling  matrix
uniform mat4 normalMatrix;	// Normal transformation matrix

// Uniform to constrian the number of lights the application uses
uniform int numLights;

// Structure for a light source. Allow up to 8 lights. No attenuation or spotlight
// support yet.
const int MAX_LIGHTS = 8; 
struct LightSource
{
	int  enabled;
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};
uniform LightSource lights[MAX_LIGHTS]; 

// Convenience method to compute the ambient, diffuse, and specular
// contribution of directional light source i
void directionalLight(in int i, in vec3 N, in vec3 vtx, in vec3 V, in float shininess,
                      inout vec4 ambient, inout vec4 diffuse, 
                      inout vec4 specular)
{
   // Add light source ambient
   ambient += lights[i].ambient;
   
   // Normalize the light direction. Note: we could use a varying parameter 
   // to pass in the normalized light source direction. However, the cost of
   // doing the perspective correct interpolation may be higher than just
   // getting the vector here.
   vec3 L = normalize(lights[i].position.xyz);
   
   // Dot product of normal and light direction
   float nDotL = dot(N, L);
   if (nDotL > 0.0)
   {
      // Add light source diffuse modulated by NDotL
      diffuse += lights[i].diffuse * nDotL;
      
      // Construct the halfway vector - note that we are assuming local viewpoint
      vec3 H = normalize(L + V);
      
      // Add specular contribution (if N dot H > 0)
      float nDotH = dot(N, H);
      if (nDotH > 0.0)
         specular += lights[i].specular * pow(nDotH, shininess);
   }
}

// Convenience method to compute the ambient, diffuse, and specular
// contribution of point light source i
void pointLight(in int i, in vec3 N, in vec3 vtx, in vec3 V, in float shininess,
                inout vec4 ambient, inout vec4 diffuse, inout vec4 specular)
{
   // Construct a vector from the vertex to the light source. Find the length for
   // use in attenuation. Normalize that vector (L)
   // distance 
   vec3 L = normalize(lights[i].position.xyz - vtx);

   // Add the light source ambient contribution
   ambient += lights[i].ambient;
   
   // Determine dot product of normal with L. If < 0 the light is not 
   // incident on the front face of the surface.
   float nDotL = dot(N, L);
   if (nDotL > 0.0)
   {
      // Add diffuse contribution of this light source
      diffuse  += lights[i].diffuse  * nDotL;
      
      // Construct the halfway vector
      vec3 H = normalize(L + V);
      
      // Add specular contribution (if N dot H > 0)
      float nDotH = dot(N, H);
      if (nDotH > 0.0)
         specular += lights[i].specular * pow(nDotH, shininess);

/**    
      // Lets use the reflection vector!
      vec3 R = reflect(-L, N);
      
      // Add specular contribution (if R dot V > 0)
      float rDotV = dot(R, V);
      if (rDotV > 0.0)
         specular += lights[i].specular * attenuation * pow(rDotV, shininess);
**/
    }
}

// Simple shader for per vertex lighting.
void main()
{
	// Transform normal and position to world coords. 
	vec3 N = normalize(vec3(normalMatrix * vec4(vertexNormal, 0.0)));
	vec3 v = vec3((modelMatrix * vec4(vertexPosition, 1.0)));

	// Construct a unit length vector from the vertex to the camera  
	vec3 V = normalize(cameraPosition - v);

	// Iterate through all lights to determine the illumination striking this vertex
	vec4 ambient  = vec4(0.0);
	vec4 diffuse  = vec4(0.0);
	vec4 specular = vec4(0.0);
	for (int i = 0; i < numLights; i++)
	{
		if (lights[i].enabled == 1)
		{
			if (lights[i].position.w == 0.0)
				directionalLight(i, N, v, V, materialShininess, ambient, diffuse, specular);
			else
				pointLight(i, N, v, V, materialShininess, ambient, diffuse, specular);
		}
	}

	// Compute color. Emmission + global ambient contribution + light sources ambient, diffuse,
	// and specular contributions
	color = materialEmission + globalLightAmbient * materialAmbient +
			(ambient  * materialAmbient) + (diffuse  * materialDiffuse) + (specular * materialSpecular);

	// Convert position to clip coordinates and pass along
	gl_Position = pvm * vec4(vertexPosition, 1.0);
}
