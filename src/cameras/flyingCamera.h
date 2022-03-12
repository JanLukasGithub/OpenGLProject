class FlyingCamera;

#ifndef CAMERAS_FLYINGCAMERA_H_
#define CAMERAS_FLYINGCAMERA_H_

#include "fpsCamera.h"
#include "../sdlEventHandler.h"

class FlyingCamera : public FpsCamera {
public:
        // Setting this to a negative value reverses vertical camera movement
        static constexpr float verticalSpeed = 5.0f;

        FlyingCamera(float fov, float width, float height);

        virtual void handleInputs(SdlEventHandler* handler, float32 delta) override;

        // Negative amount means moving down
        void moveUp(float amount);
};

#endif /* CAMERAS_FLYINGCAMERA_H_ */
