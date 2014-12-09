#version 150

// Phong shading. Fragment shader.

out vec4 fragColor;

// Incoming, interpolated normal and vertex position in world coordinates
smooth in vec3 normal;
smooth in vec3 vertex;
smooth in vec2 textureCoord;

// texture object
uniform sampler2D texture;

// Uniforms for material properties
uniform vec4   materialAmbient;
uniform	vec4   materialDiffuse;
uniform	vec4   materialSpecular;
uniform	vec4   materialEmission;
uniform	float  materialShininess;

// Global lighting environment ambient intensity
uniform vec4  globalLightAmbient;

// Camera position in world coordinates
uniform vec3  cameraPosition;

// Number of active lights
uniform int numLights;

// Structure for a light source. Allow up to 8 lights.
const int MAX_LIGHTS = 8; 
struct LightSource
{
	int  enabled;
	int  spotlight;
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
	float spotCosCutoff;
	float spotExponent;
	vec3  spotDirection;
};
uniform LightSource lights[MAX_LIGHTS]; 

// Convenience method to compute attenuation for the ith light source
// given a distance
float calculateAttenuation(in int i, in float distance)
{
	return (1.0 / (lights[i].constantAttenuation +
                  (lights[i].linearAttenuation    * distance) +
                  (lights[i].quadraticAttenuation * distance * distance)));
}

// Convenience method to compute the ambient, diffuse, and specular
// contribution of directional light source i
void directionalLight(in int i, in vec3 N, in vec3 vtx, in vec3 V, inout vec4 ambient, 
				      inout vec4 diffuse, inout vec4 specular)
{
   // Add light source ambient
   ambient += lights[i].ambient;
   
   // Get the light direction in world coordinates. Directional lights have
   // constant L (does not vary based on vertex position)- assume we have
   // normalized the light vector in the application
   vec3 L = lights[i].position.xyz;
   
   // Dot product of normal and light direction
   float nDotL = dot(N, L);
   if (nDotL > 0.0)
   {
      // Add light source diffuse modulated by NDotL
      diffuse += lights[i].diffuse * nDotL;
      
      // Construct the halfway vector - note that we are assuming local viewpoint
      vec3 H = normalize(L + V);
      
      // Find dot product of N and H and add specular contribution due to this light source
      float nDotH = dot(N, H);
      if (nDotH > 0.0)
         specular += lights[i].specular * pow(nDotH, materialShininess);
   }
}

// Convenience method to compute the ambient, diffuse, and specular
// contribution of point light source i
void pointLight(in int i, in vec3 N, in vec3 vtx, in vec3 V, inout vec4 ambient,
			    inout vec4 diffuse, inout vec4 specular)
{
   // Construct a vector from the vertex to the light source. Find the length for
   // use in attenuation. Normalize that vector (L) by dividing through by length
   vec3 tmp = lights[i].position.xyz - vtx;
   float dist = length(tmp);
   vec3 L = tmp * (1.0 / dist);

   // Compute attenuation
   float attenuation = calculateAttenuation(i, dist);  
   
   // Attenuate the light source ambient contribution
   ambient += lights[i].ambient * attenuation;
   
   // Determine dot product of normal with L. If < 0 the light is not 
   // incident on the front face of the surface.
   float nDotL = dot(N, L);
   if (nDotL > 0.0)
   {
      // Add diffuse contribution of this light source
      diffuse += lights[i].diffuse  * attenuation * nDotL;
      
      // Construct the halfway vector and add specular contribution (if N dot H > 0)
      vec3 H = normalize(L + V);
      float nDotH = dot(N, H);
      if (nDotH > 0.0)
         specular += lights[i].specular * attenuation * pow(nDotH, materialShininess);
    }
}

void spotLight(in int i, in vec3 N, in vec3 vtx, in vec3 V, inout vec4 ambient, 
		       inout vec4 diffuse, inout vec4 specular)
{
   // Construct a vector from the vertex to the light source. Find the length for
   // use in attenuation. Normalize that vector (L) by dividing through by length
   vec3 tmp = lights[i].position.xyz - vtx;
   float dist = length(tmp);
   vec3 L = tmp * (1.0 / dist);

   // Compute attenuation
   float attenuation = calculateAttenuation(i, dist);  
      
   // Determine dot product of normal with L. If < 0 the light is not 
   // incident on the front face of the surface.
   float nDotL = dot(N, L);
   if (nDotL > 0.0)
   {
      // Get the spotlight effect
      float spotEffect = dot(lights[i].spotDirection, -L);
     
      // See if within the cutoff angle
      if (spotEffect > lights[i].spotCosCutoff)
      {
         attenuation *= pow(spotEffect, lights[i].spotExponent);
            
         // Add diffuse contribution of this light source
         diffuse += lights[i].diffuse  * attenuation * nDotL;

         // Construct the halfway vector and add specular contribution (if N dot H > 0)
         vec3 H = normalize(L + V);
         float nDotH = dot(N, H);
         if (nDotH > 0.0)
            specular += lights[i].specular * attenuation * pow(nDotH, materialShininess);
	  }
      else
         attenuation = 0.0;
    }
   
	// Attenuate the light source ambient contribution (note that this can
	// be modulated by the spotlight and if outside the spotlight cutoff
	// we will have no ambient contribution. (Unclear if this is correct!)
	ambient += lights[i].ambient * attenuation;
}

// Main fragment shader. 
void main()
{
	// Normalize the normal - as a linear interpolation of normals can result
	// in non unit-length vectors. Cannot directly modify the varying value, so
	// create a temporary variable here. 
	vec3 n = normalize(normal);

	// Construct a unit length vector from the vertex to the camera  
	vec3 V = normalize(cameraPosition - vertex);

	// set texture
	vec4 textureColor;
	textureColor = texture2D(texture, textureCoord);
   
   	// Iterate through all lights to determine the illumination striking this pixel. 
	// Use the uniform variable numlights passed in by the application
    // to go use only lights up to the highest number specified.
	vec4 ambient  = vec4(0.0);
	vec4 diffuse  = vec4(0.0);
	vec4 specular = vec4(0.0);
	for (int i = 0; i < numLights; i++)
	{
		if (lights[i].enabled != 1)
			continue;

		if (lights[i].position.w == 0.0)
			directionalLight(i, n, vertex, V, ambient, diffuse, specular);
		else if (lights[i].spotlight == 1)
			spotLight(i, n, vertex, V, ambient, diffuse, specular);
		else
			pointLight(i, n, vertex, V, ambient, diffuse, specular);
   }

	// Compute color. Texture * all of Emmission + global ambient contribution + light sources ambient, diffuse,
	// and specular contributions
	vec4 color = (textureColor!=vec4(0.0) ? textureColor : vec4(1.0)) * 
			(materialEmission + globalLightAmbient * materialAmbient +
			(ambient  * materialAmbient) + 
			(diffuse  * materialDiffuse) + 
			(specular * materialSpecular));
 
	fragColor = clamp(color, 0.0, 1.0);
}
