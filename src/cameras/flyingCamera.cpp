#include "flyingCamera.h"

FlyingCamera::FlyingCamera(float fov, float width, float height) : FpsCamera(fov, width, height) {}

void FlyingCamera::handleInputs(SdlEventHandler* handler, float32 delta) {
    if (handler->keyPressed(SDLK_SPACE))
        moveUp(delta);

    if (handler->keyPressed(SDLK_LSHIFT))
        moveUp(-delta);

    FpsCamera::handleInputs(handler, delta);
}

void FlyingCamera::moveUp(float amount) {
    translate(m_up * amount * verticalSpeed);
    update();
}
