#pragma once

#include <string>
#include <EGL/egl.h>
#include <X11/Xlib.h>
#include <tuple>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

using namespace std;

class GraphisWindowRAII
{
public:
    GraphisWindowRAII(string contextType = "glx") : _contextType{contextType}
    {
        if (contextType == "egl"s)
        {
            // Use egl context/surface
            if (!SetupEGLResources(make_tuple(0, 0, 16, 16, "gl-enc"s)))
            {
                throw std::runtime_error("Failed to setup egl window.\n");
            }
        }
        else
        {
            // Use glx context/surface
            if (!SetupGLXResources())
            {
                throw std::runtime_error("Failed to setup glx window.\n");
            }
        }
    }

    virtual ~GraphisWindowRAII()
    {
        if (_contextType == "egl"s)
        {
            DestroyEGLResources();
        }
        else
        {
            DestroyGLXResources();
        }
    }

protected:
    // origin, w, h, name
    bool SetupEGLResources(tuple<int, int, int, int, string> window)
    {
        // for egl display config
        EGLint configAttrs[] =
            {
                EGL_RED_SIZE, 1,
                EGL_GREEN_SIZE, 1,
                EGL_BLUE_SIZE, 1,
                EGL_DEPTH_SIZE, 16,
                EGL_SAMPLE_BUFFERS, 0,
                EGL_SAMPLES, 0,
                EGL_CONFORMANT, EGL_OPENGL_BIT,
                EGL_NONE};
        // for egl opengl rendering context
        EGLint contextAttrs[] =
            {
                EGL_CONTEXT_CLIENT_VERSION, 3,
                EGL_NONE};
        // for window surface attributes
        EGLint windowAttrs[] = {EGL_NONE};
        EGLint configCount;

        _display = XOpenDisplay(NULL);

        if (!_display)
        {
            std::cout << "\nError opening X display\n";
            return false;
        }
        // return the default screen number
        _screen = DefaultScreen(_display);
        // EGL display for display connection
        _eglDisplay = eglGetDisplay(_display);
        if (_eglDisplay == EGL_NO_DISPLAY)
        {
            std::cout << "\nEGL : failed to obtain display connection\n";
            return false;
        }
        // initialized the EGL display connection
        if (!eglInitialize(_eglDisplay, 0, 0))
        {
            std::cout << "\nEGL : failed to display initialize\n";
            return false;
        }
        // Returns the number of frame buffer configurations returned.
        if (!eglChooseConfig(_eglDisplay, configAttrs, NULL, 0, &configCount) || !configCount)
        {
            std::cout << "\nEGL : failed to return any matching frame buffer configurations\n";
            return false;
        }

        vector<EGLConfig> configs(configCount);
        if (!eglChooseConfig(_eglDisplay, configAttrs, configs.data(), configCount, &configCount))
        {
            std::cout << "\nEGL : failed to populate frame buffer configuration list\n";
            return false;
        }
        // struct type
        auto [xpos, ypos, width, height, name] = window;
        _win = XCreateSimpleWindow(_display, RootWindow(_display, _screen),
                                   xpos, ypos, width, height, 0,
                                   BlackPixel(_display, _screen),
                                   WhitePixel(_display, _screen));

        _eglSurface = eglCreateWindowSurface(_eglDisplay, configs[0],
                                             (EGLNativeWindowType)_win, windowAttrs);

        if (!_eglSurface)
        {
            std::cout << "\nEGL : couldn't create egl surface\n";
            return false;
        }
        // for the calling thread
        eglBindAPI(EGL_OPENGL_API);
        _eglContext = eglCreateContext(_eglDisplay, configs[0], NULL, contextAttrs);

        if (!_eglContext)
        {
            std::cout << "\nEGL : couldn't create context\n";
            return false;
        }
        // attach an EGL rendering context to EGL surfaces
        if (!eglMakeCurrent(_eglDisplay, _eglSurface, _eglSurface, _eglContext))
        {
            std::cout << "\nEGL : couldn't make context/surface current\n";
            return false;
        }
        return true;
    }

    bool SetupGLXResources()
    {
        int argc = 1;
        char *argv[1] = {(char *)"dummy"};

        // Use glx context/surface
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
        glutInitWindowSize(1920, 1080);

        _win = glutCreateWindow("gl-enc");
        if (!_win)
        {
            std::cout << "\nUnable to create GLUT window.\n";
            return false;
        }
        //hide the window
        //glutHideWindow();
        return true;
    }

    void DestroyEGLResources()
    {
        if (_eglDisplay != EGL_NO_DISPLAY)
        {
            eglMakeCurrent(_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

            if (_eglContext != EGL_NO_CONTEXT)
            {
                // client send request to x11 server
                eglDestroyContext(_eglDisplay, _eglContext);
            }

            if (_eglSurface != EGL_NO_SURFACE)
            {
                // client send request to x11 server
                eglDestroySurface(_eglDisplay, _eglSurface);
            }

            eglTerminate(_eglDisplay);
        }

        if (_win)
        {
            XDestroyWindow(_display, _win);
        }

        XCloseDisplay(_display);
    }

    void DestroyGLXResources()
    {
        glutDestroyWindow(_win);
    }

private:
    string _contextType;
    Display *_display; // x11 display
    int _screen;       // x11 screen handle;
    EGLDisplay _eglDisplay{EGL_NO_DISPLAY};
    EGLSurface _eglSurface{EGL_NO_SURFACE}; // suface for presentation
    EGLContext _eglContext{EGL_NO_CONTEXT}; // EGL rendering context for the current rendering API
    // X11 Window handle
    Window _win{0};
};