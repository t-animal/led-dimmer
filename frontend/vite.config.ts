import { defineConfig } from "vite"
import { viteSingleFile } from "vite-plugin-singlefile"
import { createHtmlPlugin } from "vite-plugin-html"

export default defineConfig({
	plugins: [
		createHtmlPlugin({
			minify: true,
		}),
		viteSingleFile({ removeViteModuleLoader: true })
	],
})