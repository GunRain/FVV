import { type InferPageType } from 'fumadocs-core/source'

import { docsConfig, source } from '@/lib/source'

export const getLLMText = async (page: InferPageType<typeof source>) =>
	`# [${page.data.title}](${docsConfig.baseUrl}${page.url})${page.data.description ? `\n\n> ${page.data.description}` : ''}${await page.data.getText('processed')}`