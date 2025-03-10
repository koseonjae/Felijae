#pragma once

#include <Base/ImageFormat.h>

#include <QuartzCore/CAMetalLayer.hpp>

namespace larco {
MTL::PixelFormat getMetalImageFormat(ImageFormat format);

ImageFormat getImageFormat(MTL::PixelFormat format);
} // namespace larco
