#include <SDLWrapper/OpenGLSDLWrapper.h>


OpenGLSDLWrapper::OpenGLSDLWrapper(int width, int height)
  : m_width(width)
  , m_height(height) {
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  m_window = SDL_CreateWindow("SDL OpenGL",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              m_width,
                              m_height,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  m_context = SDL_GL_CreateContext(m_window);
}

OpenGLSDLWrapper::~OpenGLSDLWrapper() {
  SDL_GL_DeleteContext(m_context);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

std::tuple<int, int> OpenGLSDLWrapper::getDrawableSize() const {
  return {m_width, m_height};
}

SDL_PixelFormatEnum OpenGLSDLWrapper::getPixelFormat() const {
  return SDL_PIXELFORMAT_RGBA8888;
}

void OpenGLSDLWrapper::onBlitCopyAndSwapBuffer() {
  m_blitCopyCallback(nullptr);
  SDL_GL_SwapWindow(m_window);
}
