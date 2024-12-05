#pragma once

namespace goala {
class Texture;
class CommandQueue;

void blitTextureToDrawable(Texture* texture, CommandQueue* commandQueue, void* drawable);
}
