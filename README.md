# napi_openvg

The goal of this library provides OpenVG bindings for node using N-API.  
It is heavily inspired in [node-openvg](https://github.com/luismreis/node-openvg).  
napi_openvg is designed for working with [napi_sdl2](https://github.com/yubin1026/napi_sdl2);  


## Supported OS
Currently only macOS but will support others soon.


## Installation

```javascript
brew install SDL2
brew install shivavg
```

```javascript
npm install napi_sdl2
npm install napi_openvg
```

## Usage

```javascript
const SDL2 = require('napi_sdl2');
const VG = require('napi_openvg');
```

## Example
Seel more SDL examples in [napi_sdl2](https://github.com/yubin1026/napi_sdl2);

You need to set some attributes for Open GL suport with SDL2
```javascript
SDL2.SDL_GL_SetAttribute(SDL2.SDL_GL_CONTEXT_FLAGS, SDL2.SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
SDL2.SDL_GL_SetAttribute(SDL2.SDL_GL_DOUBLEBUFFER, 1);
SDL2.SDL_GL_SetAttribute(SDL2.SDL_GL_MULTISAMPLEBUFFERS, 8);
SDL2.SDL_GL_SetAttribute(SDL2.SDL_GL_MULTISAMPLESAMPLES, 8);
SDL2.SDL_GL_SetAttribute(SDL2.SDL_GL_DEPTH_SIZE, 24);
SDL2.SDL_GL_SetAttribute(SDL2.SDL_GL_STENCIL_SIZE, 8);
SDL2.SDL_GL_SetAttribute(SDL2.SDL_GL_CONTEXT_MAJOR_VERSION, 2);
SDL2.SDL_GL_SetAttribute(SDL2.SDL_GL_CONTEXT_MINOR_VERSION, 1);
```

Create SDL window with Open GL support.
```javascript
let sdl_window = SDL2.SDL_CreateWindow("OpenVG Sample", 
	0, 0, screen_width, screen_height, SDL2.SDL_WINDOW_OPENGL | SDL2.SDL_WINDOW_SHOWN | SDL2.SDL_WINDOW_RESIZABLE);
let sdl_context = SDL2.SDL_GL_CreateContext( sdl_window );
SDL2.SDL_GL_SetSwapInterval(1);
```

Create Open VG context based on Open GL.
```javascript
VG.vgCreateContextSH(screen_width, screen_height);
```

Do your Open VG job.
```javascript
let clearColor = [1,1,1,1];
VG.vgSetfv(VG.VG_CLEAR_COLOR, 4, clearColor);
VG.vgClear(x, y, w, h);
```
Flush Open VG and call GL_SwapWindow.
```javascript
VG.vgFlush();
SDL2.SDL_GL_SwapWindow( ctx.window );
```

## License

(The MIT License)

Copyright (c) 2018 Yubin

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



