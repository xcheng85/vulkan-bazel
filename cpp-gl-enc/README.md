# Opengl + nvenc

## Install glew
```shell
sudo apt-get install -y libglew-dev
```

## xlib11

X Window System protocol client library

## egl

## Link static library

"-lGL", "-lglut", "-lGLU", "-lGLEW", "-lX11", "-lEGL"

## Build 

```shell
bazel build --compilation_mode=dbg  //cpp-gl-enc
```
