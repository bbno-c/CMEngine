#pragma once

#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

#include<filesystem>
namespace fs = std::filesystem;

#include <VirtualFileSystem.hpp>
#include <Timer.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SDL3/SDL_events.h>

#include <glew.h>

#ifdef WIN32
#include <Windows.h>
#endif // WIN32

#include <GL/glu.h>

#ifdef DEBUG
#define CHECK_GL_ERROR() \
    do { \
        GLenum err = glGetError(); \
        if (err != GL_NO_ERROR) { \
            const char* errorStr = reinterpret_cast<const char*>(gluErrorString(err)); \
            std::cerr << "OpenGL error: " << errorStr << " (" << err << ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
            assert(false); \
        } \
    } while (0)

#define GL_CALL(x) \
    do { \
        x; \
        CHECK_GL_ERROR(); \
    } while (0)
#else
#define CHECK_GL_ERROR()
#define GL_CALL(x) x
#endif
