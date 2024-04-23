// vite.config.ts
import { defineConfig } from "vite";
import preact from "@preact/preset-vite";
import { espViteBuild } from "@ikoz/vite-plugin-preact-esp32";

export default defineConfig({
  plugins: [
    espViteBuild({
      enforce: "pre",
      logging: true,
    }),
    preact(),
  ],
});
