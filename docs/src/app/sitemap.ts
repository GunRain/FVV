import type { MetadataRoute } from 'next'

import { docsConfig, source } from '@/lib/source'

export const revalidate = false

export default () =>
	source
		.getPages()
		.filter(page => !page.data['go-import'] && !page.data['go-source'])
		.map(
			page =>
				({
					url: `${docsConfig.baseUrl}${page.url}`,
					lastModified: page.data.lastModified ?? new Date(),
					changeFrequency: 'always',
					priority: page.url === '/' ? 1 : 0.88
				}) satisfies MetadataRoute.Sitemap[number]
		) satisfies MetadataRoute.Sitemap