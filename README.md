# vite-plugin-esp32-web
A vite plugin to compile websites to a static header file that can be used to serve a SPA from an ESP32. All this plugin does is takes the output from the build process and converts all static files (html, css, js, media, etc.) into a single `static_files.h` that can be used on an ESP32.




## Usage with Preact
1. Create a Vite-powered Preact app  
  `npm init preact project-name`
   - >Project type:  
     >\> Single Page Application (client-side only)
   - >Project language:  
     > \> Typescript (recommended)
   - >Use router?  
     >\> Optional (see below)
3. `cd project-name`
4. `npm i vite-plugin-esp32-web`
5. Setup 

## Framework support
This plugin was created specifically for Preact with Vite due to it's small dependency size (3kb) making it suitable for the ESP32 due to limited flash. However there is nothing specific to Preact meaaning this can be used for any static website. SSR is not supported! 
