#version 450

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
	// Needs to be normalized
	vec4 direction;

	vec4 diffuse;
	vec4 specular;
	vec4 ambient;
};

struct PointLight {
	vec4 position;

	vec4 diffuse;
	vec4 specular;
	vec4 ambient;

	float linear;
	float quadratic;

	// Needed for alignment to be a multiple of 16 (namely 80)
	float alignment1, alignment2;
};

struct SpotLight {
	vec4 position;
	// Needs to be normalized
	vec4 direction;

	vec4 diffuse;
	vec4 specular;
	vec4 ambient;

	float linear;
	float quadratic;

	float innerCone;
	float outerCone;
};

layout(std430, binding = 1) buffer directionalLightBlock {
    DirectionalLight directionalLights[];
} b_directionalLights;

layout(std430, binding = 2) buffer pointLightBlock {
    PointLight pointLights[];
} b_pointLights;

layout(std430, binding = 3) buffer spotLightBlock {
    SpotLight spotLights[];
} b_spotLights;

uniform Material u_material;
uniform SpotLight u_spotLight;
uniform sampler2D u_diffuseMap;
uniform sampler2D u_normalMap;
uniform bool u_hasDiffuseMap;
uniform bool u_hasNormalMap;

vec3 view;
vec3 normal;
vec4 diffuseColor;

vec3 light;
vec3 reflection;

vec3 ambient;
vec3 diffuse;
vec3 specular;

float distance;
float attenuation;

void directionalLight() {
	for (int i = 0; i < b_directionalLights.directionalLights.length(); i++) {
		DirectionalLight dirLight = b_directionalLights.directionalLights[i];

		light = normalize(dirLight.direction.xyz);
		reflection = reflect(dirLight.direction.xyz, normal);
		
		ambient = diffuseColor.xyz * dirLight.ambient.xyz;
		diffuse = max(dot(normal, light), 0.0) * diffuseColor.xyz * dirLight.diffuse.xyz;
		specular = pow(max(dot(reflection, view), 0.000001), u_material.shininess) * u_material.specular * dirLight.specular.xyz;
	}
}

void pointLight() {
	for (int i = 0; i < b_pointLights.pointLights.length(); i++) {
		PointLight pointLight = b_pointLights.pointLights[i];

		light = normalize(pointLight.position.xyz - v_position);
		reflection = reflect(-light, normal);
		
		distance = length(pointLight.position.xyz - v_position);
		attenuation = 1.0f / ((1.0f) + (pointLight.linear * distance) + (pointLight.quadratic * distance * distance));
		
		ambient += attenuation * diffuseColor.xyz * pointLight.ambient.xyz;
		diffuse += attenuation * max(dot(normal, light), 0.0) * diffuseColor.xyz * pointLight.diffuse.xyz;
		specular += attenuation * pow(max(dot(reflection, view), 0.000001), u_material.shininess) * u_material.specular * pointLight.specular.xyz;
	}
}

void spotLight() {
	for (int i = 0; i < b_spotLights.spotLights.length(); i++) {
		SpotLight spotLight = b_spotLights.spotLights[i];

		light = normalize(spotLight.position.xyz - v_position);
		reflection = reflect(-light, normal);
		float theta = dot(light, spotLight.direction.xyz);
		
		distance = length(spotLight.position.xyz - v_position);
		attenuation = 1.0f / ((1.0f) + (spotLight.linear * distance) + (spotLight.quadratic * distance * distance));
		
		float epsilon = spotLight.innerCone - spotLight.outerCone;
		float intensity = clamp((theta - spotLight.outerCone) / epsilon, 0.0f, 1.0f) * int(theta > spotLight.outerCone);
		
		diffuse += intensity * attenuation * max(dot(normal, light), 0.0) * diffuseColor.xyz * spotLight.diffuse.xyz;
		specular += intensity * attenuation * pow(max(dot(reflection, view), 0.000001), u_material.shininess) * u_material.specular * spotLight.specular.xyz;
		ambient += attenuation * spotLight.ambient.xyz * diffuseColor.xyz;
	}
}

void main() {
	// Vector from fragment to camera (camera always at 0,0,0)
	view = normalize(-v_position);
	
	normal = int(u_hasNormalMap) * texture(u_normalMap, v_textureCoords).rgb + int(!u_hasNormalMap) * vec3(0.5, 0.5, 1.0);
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(v_tbn * normal);
	
	diffuseColor = int(u_hasDiffuseMap) * texture(u_diffuseMap, v_textureCoords) + int(!u_hasDiffuseMap) * vec4(u_material.diffuse, 1.0);
	if (diffuseColor.w < 0.9) {
		discard;
	}

	directionalLight();
	pointLight();
	spotLight();

    f_color = vec4(ambient + diffuse + specular + u_material.emissive, diffuseColor.w);
}