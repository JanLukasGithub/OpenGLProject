#version 330 core

// f_ means fragment
layout(location = 0) out vec4 f_color;

// v_ means vertex
in vec3 v_position;
in vec2 v_textureCoords;
in mat3 v_tbn;

struct Material {
	vec3 diffuse;
	vec3 specular;
	vec3 emissive;
	float shininess;
};

struct DirectionalLight {
	vec3 direction;
	
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
};

struct PointLight {
	vec3 position;
	
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
	
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 position;
	// Needs to be normalized
	vec3 direction;
	
	float linear;
	float quadratic;
	
	float innerCone;
	float outerCone;
	
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
};

uniform mat4 u_modelView;

uniform Material u_material;
uniform DirectionalLight u_directionalLight;
uniform PointLight u_pointLight;
uniform SpotLight u_spotLight;
uniform sampler2D u_diffuseMap;
uniform sampler2D u_normalMap;
uniform bool u_hasDiffuseMap;

void main() {
	// Vector from fragment to camera (camera always at 0,0,0)
	vec3 view = normalize(-v_position);
	
	// Normal from normal map
	vec3 normal = texture(u_normalMap, v_textureCoords).rgb;
	// Scale from 0.0 - 1.0 to -1.0 - 1.0
	normal = normalize(normal * 2.0 - 1.0);
	// Actual normal
	normal = normalize(v_tbn * normal);
	
	// Get diffuse color from material or texture
	vec4 diffuseColor = vec4(u_material.diffuse, 1.0);
	if (u_hasDiffuseMap) {
		diffuseColor = texture(u_diffuseMap, v_textureCoords);
	}
	if (diffuseColor.w < 0.9) {
		discard;
	}

	// Transform direction to view space
	vec3 directionalLightDirection = -(u_directionalLight.direction * mat3(u_modelView));
	
	// Directional light
	vec3 light = normalize(directionalLightDirection);
	vec3 reflection = reflect(directionalLightDirection, normal);
	
	vec3 ambient = diffuseColor.xyz * u_directionalLight.ambient;
	vec3 diffuse = max(dot(normal, light), 0.0) * diffuseColor.xyz * u_directionalLight.diffuse;
	vec3 specular = pow(max(dot(reflection, view), 0.000001), u_material.shininess) * u_material.specular * u_directionalLight.specular;

	// Point light
	light = normalize(u_pointLight.position - v_position);
	reflection = reflect(-light, normal);
	
	float distance = length(u_pointLight.position - v_position);
	float attenuation = 1.0f / ((1.0f) + (u_pointLight.linear * distance) + (u_pointLight.quadratic * distance * distance));
	
	ambient += attenuation * diffuseColor.xyz * u_pointLight.ambient;
	diffuse += attenuation * max(dot(normal, light), 0.0) * diffuseColor.xyz * u_pointLight.diffuse;
	specular += attenuation * pow(max(dot(reflection, view), 0.000001), u_material.shininess) * u_material.specular * u_pointLight.specular;

	// Spotlight
	light = normalize(u_spotLight.position - v_position);
	reflection = reflect(-light, normal);
	float theta = dot(light, u_spotLight.direction);
	
	distance = length(u_spotLight.position - v_position);
	attenuation = 1.0f / ((1.0f) + (u_spotLight.linear * distance) + (u_spotLight.quadratic * distance * distance));
	
	if (theta > u_spotLight.outerCone) {
		float epsilon = u_spotLight.innerCone - u_spotLight.outerCone;
		float intensity = clamp((theta - u_spotLight.outerCone) / epsilon, 0.0f, 1.0f);
	
		ambient += attenuation * u_spotLight.ambient * diffuseColor.xyz;
		diffuse += intensity * attenuation * max(dot(normal, light), 0.0) * diffuseColor.xyz * u_spotLight.diffuse;
		specular += intensity * attenuation * pow(max(dot(reflection, view), 0.000001), u_material.shininess) * u_material.specular * u_spotLight.specular;
	} else {
		ambient += attenuation * u_spotLight.ambient * diffuseColor.xyz;
	}

	// Final color
    f_color = vec4(ambient + diffuse + specular + u_material.emissive, 1.0f);
}
