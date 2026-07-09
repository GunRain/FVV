import { type MetadataRoute } from 'next'

import { docsConfig } from '@/lib/source'

export const revalidate = false

export default () =>
	({
		rules: {
			userAgent: '*',
			allow: '/'
		},
		host: new URL(docsConfig.baseUrl).hostname,
		sitemap: `${docsConfig.baseUrl}/sitemap.xml`
	}) satisfies MetadataRoute.Robots