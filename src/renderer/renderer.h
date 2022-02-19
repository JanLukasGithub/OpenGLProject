#ifndef RENDERER_H_
#define RENDERER_H_

#include "../utils.h"

class Renderer {
public:
    // Initializes rendering. After calling this, you should be able to render anything without more initializing to do
    virtual void init() = 0;
    // Resets rendering. Enables you to "hot swap" shaders or potentially recover from rendering issues
    virtual void reset() = 0;
    // Resets Screen for rendering the next frame
    virtual void startFrame() = 0;
    // Sets up shader and render library settings for rendering 3d models
    virtual void setup3DRender() = 0;
    // Sets up shader and render library settings for rendering fonts
    virtual void setupFontRender() = 0;
    // Draws things on the screen
    virtual void endFrame() = 0;

    // FPS are calculated each time endFrame() is called
    virtual uint32 getFPS() = 0;
    // Delta is calculated each time endFrame() is called
    virtual float32 getDelta() = 0;
};

#endif