# radwm

An X11 window manager written in C.

Still very early/work in progress. The goal is to have something clean and hackable,
in the spirit of dwm but with better organized code, and centralized arena style memory management and cache coherency.
Basically think of dwm if the guy who wrote it seen Mike Actons 2014 Cpp con talk first.

RAD stands for (Rapid Application Development) or (Research and Development) or whatever you want, I'm using it because of
the raddebugger project is awesome and I like the way it sounds, thats it.

## Dependencies

You'll need a C compiler (clang or gcc), Xlib headers, and Python 3.

```
# Arch
sudo pacman -S clang gcc libx11 libxrandr

# Debian/Ubuntu
sudo apt install clang gcc libx11-dev libxrandr-dev
```

## Building

The build system is a simple Python script. Clang is the default compiler (because it optimizes better than gcc)

```
# release build (clang)
python build.py

# release build (gcc)
python build.py --gcc

# debug build with sanitizers
python build.py --debug

```

## Tests

```
python build.py --tests
```
