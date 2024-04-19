// vite.config.ts
import { defineConfig } from "vite";
import preact from "@preact/preset-vite";
import { espViteBuild } from "vite-plugin-esp32-web";

export default defineConfig({
  plugins: [espViteBuild(), preact()],
});
