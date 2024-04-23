# vite-plugin-preact-esp32
A vite plugin to compile websites to a static header file that can be used to serve a SPA from an ESP32. All this plugin does is takes the output from the build process and converts all static files (html, css, js, media, etc.) into a single `static_files.h` that can be used on an ESP32.




## Usage with Preact
1. Create a Vite-powered Preact app  
  `npm init preact`
   - >Project type:  
     >\> Single Page Application (client-side only)
   - >Project language:  
     > \> Typescript (recommended)
   - >Use router?  
     >\> Optional (see below)
2. `cd project-name`
3. `npm i -D vite-plugin-preact-esp32`
4. Add the plugin to the vite config
   ```ts
    // vite.config.ts
    import { defineConfig } from "vite";
    import preact from "@preact/preset-vite";
    import { espViteBuild } from "vite-plugin-preact-esp32";
    
    export default defineConfig({
      plugins: [espViteBuild(), preact()],
    });
    ```
5. `npm run build` or `npx vite build`
6. Under `./dist/_esp32` you will have your output `static_files.h`. 

### Using the router
Whether or not you use the router is optional. If you want more than one page, you very likely want to use it. It only adds a few extra kB's, so there isn't really a reason _not_ to use it unless you just have a single page.  

:warning: If you use this with an SPA (Preact for example), you will lose context if you refresh the page. I.e. if you navigate to `example.com/test` from a link within the Router, all is well. However, if you navigate directly to `example.com/test` or refresh the page, you will be served a 404 from the webserver (not from the SPA). [Check here](https://stackoverflow.com/a/43557288/10422604) for a better explanation. I will update this when I find a solution.


# ESP32 Setup
Once you have your `static_files.h`, it's up to you how you access it and serve it from your ESP32. There are examples in /esp32 for both Arduino and PlatformIO. 

## Arduino
This is the general setup for the WebServer library by Ivan Grokohotkov. This can be used with any library, but the setup may vary. The general idea will be the same though.

You will notice at the bottom of the `static_files.h` there is something that looks like the following:  
```c++
  const file files[] PROGMEM = {
      {.path = "/assets/preact-48177e6f.svg",
          .size = f_assets_preact_48177e6f_svg_size,
          .type = "image/svg+xml",
          .contents = f_assets_preact_48177e6f_svg_contents},
    
      {.path = "/assets/index-f615e9a0.css",
          .size = f_assets_index_f615e9a0_css_size,
          .type = "text/css",
          .contents = f_assets_index_f615e9a0_css_contents},
    
      {.path = "/assets/index-a196fdf4.js",
          .size = f_assets_index_a196fdf4_js_size,
          .type = "application/javascript",
          .contents = f_assets_index_a196fdf4_js_contents},
    
      {.path = "/index.html",
          .size = f_index_html_size,
          .type = "text/html",
          .contents = f_index_html_contents}
  };
```

This is the array of files that were compiled. On your webserver, you should loop over these and create a route for each one. This 
```c++
#include "static_files.h"
// setup()...
for (int i = 0; i < static_files::num_of_files; i++)
  {
    server.on(static_files::files[i].path, [i]
              {
      server.sendHeader("Content-Encoding", "gzip");
      server.send_P(200, static_files::files[i].type, (const char *)static_files::files[i].contents, static_files::files[i].size); });
  }
```
So now `example.com/index.html` will serve the `.contents = f_index_html_contents` from above.  

You will also probably want to manually add the `index.html` as the default entry point so that you access the index when you navigate to `example.com/`
```c++
  server.on("/", []{
    server.sendHeader("Content-Encoding", "gzip");
    server.send_P(200, "text/html", (const char *)static_files::f_index_html_contents, static_files::f_index_html_size);
  });
```

## PlatformIO

Make sure you include `#include <pgmspace.h>` alongside the `#include "static_files.h"` to ensure it compiles. This is due to the `PROGMEM` in the `static_files.h`. In the future there will be a config and separate templates, but for now this will do. Alternatively just remove `PROGMEM` from the file.  
Other than that, follow similar directions as above or refer to the example under /esp32/platformio

# Other

### How much memory does this take up? What are the advantages of using Preact?
Preact has a depencency size of 3kb making it an ideal front-end framework. Preact has a near identical API to Reac, making it easy to create reusable components and a single-page-application (SPA).  

The default template you get when you run `npm init preact` compiles down to 9.48kb!
```
dist/index.html                   0.48 kB │ gzip: 0.32 kB
dist/assets/preact-48177e6f.svg   1.59 kB │ gzip: 0.77 kB
dist/assets/index-f615e9a0.css    1.42 kB │ gzip: 0.66 kB
dist/assets/index-a196fdf4.js    19.02 kB │ gzip: 7.73 kB
```
Do not worry about the `static_files.h` filesize, as it will be roughly 5x larger (58 KB for the default template) due to formatting. Your C++ compiler will minimize it back down when you flash your ESP32.  

More complex applications will be around 100-500kB in size. Considering an ESP32-S3 has 8mB of flash, that's only 1-6% of the total flash storage.

### Will this work with server side generation/rendering?
No! Good luck getting Node to run on an ESP32. This will only work with client-sided static websites. If you need API routes or to do server stuff, check the examples out.

### Can I use this for [insert microchip here]?
Yes. The idea is the same: Convert a static website into a single header file and serve the corresponding file in a webserver. 

### Will this also work for [insert framework here]
Probably. Again, this isn't doing anything out of the ordinary. It is simply converting a static website to a header file. This is just a plugin specifically for Vite. As long as the framework is able to export a static website and is powered by Vite, it _should_ work. Personally I prefer Preact due to it's small dependency and simplicity, making it ideal for the ESP32. Feel free to submit a PR with a working example if it works for other frameworks!

## Acknowledgements
This was inspired from [https://github.com/mruettgers/preact-template-esp](https://github.com/mruettgers/preact-template-esp) which is no longer maintained and uses an outdated version of Preact.

