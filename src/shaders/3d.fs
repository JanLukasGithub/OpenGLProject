#version 450

// f_ means fragment
layout(location = 0) out vec4 f_color;

// v_ means vertex
in vec3 v_view_space_position;
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
	float alignment1;

	vec3 diffuse;
	float alignment2;
	vec3 specular;
	float alignment3;
	vec3 ambient;
	float alignment4;
};

struct PointLight {
	vec3 position;
	float linear;
	vec3 diffuse;
	float quadratic;
	vec3 specular;
	float alignment1;
	vec3 ambient;
	float alignment2;
};

struct SpotLight {
	vec3 position;
	float linear;
	vec3 direction;
	float quadratic;
	vec3 diffuse;
	float innerCone;
	vec3 specular;
	float outerCone;
	vec3 ambient;
	float alignment1;
};

layout(std430, binding = 0) buffer directionalLightBlock {
    DirectionalLight directionalLights[];
} b_directionalLights;

layout(std430, binding = 1) buffer pointLightBlock {
    PointLight pointLights[];
} b_pointLights;

layout(std430, binding = 2) buffer spotLightBlock {
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

vec3 ambient = vec3(0.0f);
vec3 diffuse = vec3(0.0f);
vec3 specular = vec3(0.0f);

void directionalLight() {
	for (int i = 0; i < b_directionalLights.directionalLights.length(); i++) {
		DirectionalLight dirLight = b_directionalLights.directionalLights[i];

		vec3 light = -dirLight.direction;
		vec3 reflection = reflect(dirLight.direction, normal);

		ambient += diffuseColor.xyz * dirLight.ambient;
		diffuse += max(dot(normal, light), 0.0) * diffuseColor.xyz * dirLight.diffuse;
		specular += pow(max(dot(reflection, view), 0.000001), u_material.shininess) * u_material.specular * dirLight.specular;
	}
}

void pointLight() {
	for (int i = 0; i < b_pointLights.pointLights.length(); i++) {
		PointLight pointLight = b_pointLights.pointLights[i];

		vec3 light = normalize(pointLight.position.xyz - v_view_space_position);
		vec3 reflection = reflect(-light, normal);
		
		float distance = length(pointLight.position.xyz - v_view_space_position);
		float attenuation = 1.0f / ((1.0f) + (pointLight.linear * distance) + (pointLight.quadratic * distance * distance));
		
		ambient += attenuation * diffuseColor.xyz * pointLight.ambient.xyz;
		diffuse += attenuation * max(dot(normal, light), 0.0) * diffuseColor.xyz * pointLight.diffuse.xyz;
		specular += attenuation * pow(max(dot(reflection, view), 0.000001), u_material.shininess) * u_material.specular * pointLight.specular.xyz;
	}
}

void spotLight() {
	for (int i = 0; i < b_spotLights.spotLights.length(); i++) {
		SpotLight spotLight = b_spotLights.spotLights[i];

		vec3 light = normalize(spotLight.position.xyz - v_view_space_position);
		vec3 reflection = reflect(-light, normal);
		float theta = dot(light, spotLight.direction.xyz);
		
		float distance = length(spotLight.position.xyz - v_view_space_position);
		float attenuation = 1.0f / ((1.0f) + (spotLight.linear * distance) + (spotLight.quadratic * distance * distance));
		
		float epsilon = spotLight.innerCone - spotLight.outerCone;
		float intensity = clamp((theta - spotLight.outerCone) / epsilon, 0.0f, 1.0f) * int(theta > spotLight.outerCone);
		
		ambient += attenuation * spotLight.ambient.xyz * diffuseColor.xyz;
		diffuse += intensity * attenuation * max(dot(normal, light), 0.0) * diffuseColor.xyz * spotLight.diffuse.xyz;
		specular += intensity * attenuation * pow(max(dot(reflection, view), 0.000001), u_material.shininess) * u_material.specular * spotLight.specular.xyz;
	}
}

void main() {
	view = normalize(-v_view_space_position);
	
	normal = int(u_hasNormalMap) * normalize(v_tbn * normalize(texture(u_normalMap, v_textureCoords).rgb * 2.0 - 1.0))
			+ int(!u_hasNormalMap) * transpose(v_tbn)[2];
	
	diffuseColor = int(u_hasDiffuseMap) * texture(u_diffuseMap, v_textureCoords) + int(!u_hasDiffuseMap) * vec4(u_material.diffuse, 1.0);
	if (diffuseColor.w < 0.9) {
		discard;
	}

	directionalLight();
	pointLight();
	spotLight();

    f_color = vec4(ambient + diffuse + specular + u_material.emissive, diffuseColor.w);
}