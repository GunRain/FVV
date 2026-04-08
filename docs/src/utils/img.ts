import { type InferPageType } from 'fumadocs-core/source'

import { source } from '@/lib/source'

export const getPageImage = (page: InferPageType<typeof source>) =>
	(segments => ({
		segments,
		url: `/og/${segments.join('/')}`,
	}))([...page.slugs, 'image.png'])
