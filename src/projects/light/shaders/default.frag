//A minimal implementation of a fragment shader

#version 430

struct Light {
  vec3  position;
  vec3  color;
  float intensity;
  float attenuation;
  float radius;
};

layout (std140, binding = 0) uniform CameraInfo {
  mat4	ProjectionView; 
  vec3	eye;
};

layout (std430, binding = 1) readonly buffer LightsData {
	Light lights[];
};

uniform vec3 ambient_light_color;
uniform float ambient_light_intensity;

in  vec3 ex_FragPos;
in  vec4 ex_Color;
in	vec3 ex_Normal;
out vec4 out_Color;

void main(void)
{
	// Basic ambient light
	vec3 ambient_light = ambient_light_color * ambient_light_intensity;

	int i;
	vec3 diffuse = vec3(0.0,0.0,0.0);
	vec3 specular = vec3(0.0,0.0,0.0);
	for (i = 0; i < lights.length(); ++i) {
		Light wLight = lights[i];
		// Basic diffuse light
		vec3 norm = normalize(ex_Normal); // in this project the normals are all normalized anyway...
		vec3 lightDir = normalize(wLight.position - ex_FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		diffuse += diff * wLight.color;
		
		// Basic specular light
		vec3 viewDir = normalize(eye - ex_FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		specular += wLight.intensity * spec * wLight.color;  
	}

	out_Color = ex_Color * vec4(specular + diffuse + ambient_light,1.0); 
}