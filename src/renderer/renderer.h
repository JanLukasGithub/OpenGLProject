#ifndef RENDERER_H_
#define RENDERER_H_

class Renderer {
    // Initializes rendering. After calling this, you should be able to render anything without more initializing to do
    virtual void init() = null;
    // Resets rendering. Enables you to "hot swap" shaders or potentially recover from rendering issues
    virtual void reset() = null;
    // Resets Screen for rendering the next frame
    virtual void startFrame() = null;
    // Sets up shader and render library settings for rendering 3d models
    virtual void setup3DRender() = null;
    // Sets up shader and render library settings for rendering fonts
    virtual void setupFontRender() = null;
    // Draws things on the screen
    virtual void endFrame() = null;
};

#endif