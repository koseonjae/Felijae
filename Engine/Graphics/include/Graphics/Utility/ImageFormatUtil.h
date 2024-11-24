#pragma once

#include <Base/ImageFormat.h>

#include <QuartzCore/CAMetalLayer.hpp>

namespace goala {

MTL::PixelFormat getMetalImageFormat(ImageFormat format);

ImageFormat getImageFormat(MTL::PixelFormat format);

} // namespace goala
