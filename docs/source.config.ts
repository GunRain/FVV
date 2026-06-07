import { metaSchema, pageSchema } from 'fumadocs-core/source/schema'
import { defineConfig, defineDocs } from 'fumadocs-mdx/config'
import lastModified from 'fumadocs-mdx/plugins/last-modified'
import { z } from 'zod'

export const docs = defineDocs({
	dir: 'content',
	docs: {
		schema: pageSchema.extend({
			keywords: z.array(z.string()).optional(),
			'go-import': z.string().optional(),
			'go-source': z.string().optional()
		}),
		postprocess: { includeProcessedMarkdown: true }
	},
	meta: { schema: metaSchema }
})

export default defineConfig({
	plugins: [lastModified()],
	mdxOptions: {}
})