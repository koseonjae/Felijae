#pragma once

namespace MTL {
class Texture;
class Device;
class CommandQueue;
}

namespace CA {
class MetalDrawable;
}

namespace goala {
void blitTextureToDrawable(MTL::Texture* texture, CA::MetalDrawable* drawable, MTL::CommandQueue* commandQueue);
}
