#include "FlyingCamera.h"

#include "../SdlEventHandler.h"
#include <iostream>

FlyingCamera::FlyingCamera(float fov, float width, float height) : FpsCamera(fov, width, height) {
}

void FlyingCamera::moveUp(float amount) {
    translate(up * amount * verticalSpeed);
    update();
}

void FlyingCamera::handleInputs(SdlEventHandler* handler, float32 delta) {
    if (handler->keyPressed(SDLK_SPACE))
        moveUp(delta);

    if (handler->keyPressed(SDLK_LSHIFT))
        moveUp(-delta);

    FpsCamera::handleInputs(handler, delta);
}
