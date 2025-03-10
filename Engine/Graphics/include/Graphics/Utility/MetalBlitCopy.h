#pragma once

namespace larco {
class Texture;
class CommandQueue;

void blitTextureToDrawable(Texture* texture, CommandQueue* commandQueue, void* drawable);
}
