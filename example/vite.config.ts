import { defineConfig } from "vite";
import preact from "@preact/preset-vite";
import { espViteBuild } from "vite-plugin-preact-esp32";

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [espViteBuild(), preact()],
});
