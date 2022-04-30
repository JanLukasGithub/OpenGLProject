#ifndef I_RENDERABLE_H_
#define I_RENDERABLE_H_

class IRenderable {
public:
    virtual void render() const noexcept = 0;
};

#endif