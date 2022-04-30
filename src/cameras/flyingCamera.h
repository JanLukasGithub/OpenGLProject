#ifndef CAMERAS_FLYINGCAMERA_H_
#define CAMERAS_FLYINGCAMERA_H_

#include "fpsCamera.h"

class FlyingCamera : public FpsCamera {
public:
    static constexpr float verticalSpeed = 5.0f;

    FlyingCamera(float fov, float width, float height);

    virtual void handleInputs(SdlEventHandler* handler, float32 delta) override;

    void moveUp(float amount);
};

#endif