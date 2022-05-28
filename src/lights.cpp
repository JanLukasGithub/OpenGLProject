#include "lights.h"

Lights::Lights() : m_directionalLightBuffer{ BUFFER_BINDING_DIRECTIONAL_LIGHT }, m_pointLightBuffer{ BUFFER_BINDING_POINT_LIGHT },
m_spotLightBuffer{ BUFFER_BINDING_SPOT_LIGHT } {}

Lights::Lights(const DirectionalLight& dirLight, const PointLight& pointLight, const SpotLight& spotLight) :
    m_directionalLightBuffer{ BUFFER_BINDING_DIRECTIONAL_LIGHT }, m_pointLightBuffer{ BUFFER_BINDING_POINT_LIGHT },
    m_spotLightBuffer{ BUFFER_BINDING_SPOT_LIGHT } {

    m_directionalLightBuffer.add(dirLight);
    m_pointLightBuffer.add(pointLight);
    m_spotLightBuffer.add(spotLight);
}

Lights::Lights(const std::vector<DirectionalLight>& dirLights, const std::vector<PointLight>& pointLights, const std::vector<SpotLight>& spotLights) :
    m_directionalLightBuffer{ BUFFER_BINDING_DIRECTIONAL_LIGHT }, m_pointLightBuffer{ BUFFER_BINDING_POINT_LIGHT },
    m_spotLightBuffer{ BUFFER_BINDING_SPOT_LIGHT } {

    m_directionalLightBuffer.add(dirLights);
    m_pointLightBuffer.add(pointLights);
    m_spotLightBuffer.add(spotLights);
}

Lights& Lights::operator=(const Lights& lights) {
    if (this == &lights)
        return *this;

    m_directionalLightBuffer = lights.m_directionalLightBuffer;
    m_pointLightBuffer = lights.m_pointLightBuffer;
    m_spotLightBuffer = lights.m_spotLightBuffer;

    return *this;
}

void Lights::bind() const noexcept {
    m_directionalLightBuffer.bind();
    m_pointLightBuffer.bind();
    m_spotLightBuffer.bind();
}

void Lights::unbind() const noexcept {
    m_directionalLightBuffer.unbind();
    m_pointLightBuffer.unbind();
    m_spotLightBuffer.unbind();
}

ShaderBuffer<DirectionalLight>& Lights::getDirectionalLightBuffer() { return m_directionalLightBuffer; }
ShaderBuffer<PointLight>& Lights::getPointLightBuffer() { return m_pointLightBuffer; }
ShaderBuffer<SpotLight>& Lights::getSpotLightBuffer() { return m_spotLightBuffer; }