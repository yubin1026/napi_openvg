# napi_openvg

The goal of this library provides OpenVG bindings for node using N-API.
It is heavily inspired in [node-openvg](https://github.com/luismreis/node-openvg). 


## Supported OS
Currently only macOS


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

```javascript

const util = require("util");
const fs = require('fs');

const SDL2 = require('napi_sdl2');
const VG = require('napi_openvg');

let context = {
	window : null
}

function clear_box(x, y, w, h)
{
    // clear screen
    let clearColor = [1,1,1,1];
    VG.vgSetfv(VG.VG_CLEAR_COLOR, 4, clearColor);
    VG.vgClear(x, y, w, h);
}

function fill_rect(x, y, w, h, bg)
{
    // draw filled rect
    let path;   
    let fillPaint;
       
    VG.vgLoadIdentity();
    VG.vgSeti(VG.VG_MATRIX_MODE, VG.VG_MATRIX_PATH_USER_TO_SURFACE);
    
    fillPaint = VG.vgCreatePaint();
    VG.vgSetParameteri(fillPaint, VG.VG_PAINT_TYPE, VG.VG_PAINT_TYPE_COLOR);
    VG.vgSetParameterfv(fillPaint, VG.VG_PAINT_COLOR, 4, bg);
    VG.vgSetPaint(fillPaint, VG.VG_FILL_PATH);
    
    path = VG.vgCreatePath(VG.VG_PATH_FORMAT_STANDARD, VG.VG_PATH_DATATYPE_F, 1,0,0,0, VG.VG_PATH_CAPABILITY_ALL);
    VG.vguRect(path, x, y, w, h);
    
    VG.vgLoadIdentity();
    VG.vgSeti(VG.VG_MATRIX_MODE, VG.VG_MATRIX_PATH_USER_TO_SURFACE);
    
    VG.vgDrawPath(path, VG.VG_FILL_PATH);
    
    VG.vgDestroyPath(path);
    VG.vgDestroyPaint(fillPaint);
}

function render(ctx)
{
	let [screen_width, screen_height] = SDL2.SDL_GetWindowSize(ctx.window);

	clear_box(0, 0, screen_width, screen_height);
	fill_rect(screen_width / 4, screen_height / 4, screen_width / 2, screen_height / 2, [1, 0, 0, 1]);

	VG.vgFlush();
	SDL2.SDL_GL_SwapWindow( ctx.window );
}

function main() {

	SDL2.SDL_Init(SDL2.SDL_INIT_EVERYTHING);
	SDL2.SDL_GL_SetAttribute(SDL2.SDL_GL_CONTEXT_FLAGS, SDL2.SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL2.SDL_GL_SetAttribute(SDL2.SDL_GL_DOUBLEBUFFER, 1);
	SDL2.SDL_GL_SetAttribute(SDL2.SDL_GL_MULTISAMPLEBUFFERS, 8);
	SDL2.SDL_GL_SetAttribute(SDL2.SDL_GL_MULTISAMPLESAMPLES, 8);
	SDL2.SDL_GL_SetAttribute(SDL2.SDL_GL_DEPTH_SIZE, 24);
	SDL2.SDL_GL_SetAttribute(SDL2.SDL_GL_STENCIL_SIZE, 8);
	SDL2.SDL_GL_SetAttribute(SDL2.SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL2.SDL_GL_SetAttribute(SDL2.SDL_GL_CONTEXT_MINOR_VERSION, 1);


	let [screen_width, screen_height] = [800, 800];
	let sdl_window = SDL2.SDL_CreateWindow("OpenVG Sample", 
		0, 0, screen_width, screen_height, SDL2.SDL_WINDOW_OPENGL | SDL2.SDL_WINDOW_SHOWN | SDL2.SDL_WINDOW_RESIZABLE);
	let sdl_context = SDL2.SDL_GL_CreateContext( sdl_window );
	SDL2.SDL_GL_SetSwapInterval(1);
   
	let quit = false;
	VG.vgCreateContextSH(screen_width, screen_height);

	context.window = sdl_window;
	render(context);
	
	let cursor = SDL2.SDL_CreateSystemCursor(SDL2.SDL_SYSTEM_CURSOR_ARROW);
	SDL2.SDL_SetCursor(cursor);
	SDL2.SDL_ShowCursor(1);
			
 	//SDL2.SDL_StartTextInput();
	while(!quit)
	{
		let event = {};
		SDL2.SDL_PumpEvents();
		while(1) {
			let ret = SDL2.SDL_PeepEvents(event, 1, SDL2.SDL_GETEVENT, SDL2.SDL_FIRSTEVENT, SDL2.SDL_LASTEVENT);
			if(ret == 1) break;
			SDL2.SDL_Delay(10);
			render(context);
			SDL2.SDL_PumpEvents();
		}

		switch(event.type)
		{
			case "MOUSEBUTTONDOWN":
				break;
			case "MOUSEBUTTONUP":
				break;
			case "MOUSEWHEEL":
				break;
			case "WINDOWEVENT":
				if(event.event == "WINDOWEVENT_RESIZED") {
					[screen_width, screen_height] = SDL2.SDL_GetWindowSize(context.window);
					VG.vgResizeSurfaceSH(screen_width, screen_height);
					render(context);
				} else if(event.event == "WINDOWEVENT_SIZE_CHANGED") {
		
				} else if(event.event == "WINDOWEVENT_EXPOSED") {
				}
				break;
			case "KEYDOWN":
				break;
			case "QUIT":
				quit = true;
				break;
		}
	}
	SDL2.SDL_DestroyWindow(context.window);
	VG.vgDestroyContextSH();
	SDL2.SDL_Quit();
}

main();

```

## License

(The MIT License)

Copyright (c) 2018 Yubin

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



