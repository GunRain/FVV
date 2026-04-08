import { type ButtonItemType, type IconItemType, type MainItemType, type MenuItemType } from 'fumadocs-ui/layouts/shared'
import { NextResponse } from 'next/server'

import { docsConfig, docsOptions, source } from '@/lib/source'

export const revalidate = false

export const GET = () => {
	const links =
		docsOptions.links &&
		(docsOptions.links.filter(
			link =>
				link.type !== 'custom' &&
				typeof link.text === 'string' &&
				(link.url ??
					(link.type === 'menu' &&
						link.items.filter(item => item.type === 'main' && typeof item.text === 'string' && item.url).length >
							0)),
		) as (MainItemType | IconItemType | ButtonItemType | MenuItemType)[])

	return new NextResponse(
		`# [${docsConfig.title}](${docsConfig.baseUrl})` +
			(((links && links.length > 0) ?? false)
				? `\n\n## Navigation\n\n` +
					links!
						.map(link => {
							const items =
								link.type === 'menu' &&
								link.items &&
								(link.items.filter(
									item => item.type === 'main' && typeof item.text === 'string' && item.url,
								) as MainItemType[])
							return `- ${link.url ? `[**${link.text}**](${link.url.startsWith('/') ? `${docsConfig.baseUrl}${link.url}` : link.url})` : `**${link.text}**`}${link.type == 'main' && link.description ? `: ${link.description}` : ''}${items && items.length > 0 ? `\n${items.map(item => `  - [${item.text}](${item.url.startsWith('/') ? `${docsConfig.baseUrl}${item.url}` : item.url})${item.description ? `: ${item.description}` : ''}`).join('\n')}` : ''}`
						})
						.join('\n')
				: '') +
			`\n\n## Pages\n\n` +
			source
				.getPages()
				.filter(page => !page.data['go-import'] && !page.data['go-source'])
				.map(
					page =>
						`- [**${page.data.title}**](${docsConfig.baseUrl}${page.url === '/' ? '/index' : page.url}.md)${page.data.description ? `: ${page.data.description}` : ''}`,
				)
				.join('\n') +
			(docsConfig.footer.links.length > 0
				? `\n\n## Links\n\n` +
					docsConfig.footer.links
						.map(
							group =>
								`- **${group.title}**\n` +
								group.items.map(item => `  - [${item.label}](${item.href})`).join('\n'),
						)
						.join('\n')
				: '') +
			`\n\n---\n\n> [**Full Content**](${docsConfig.baseUrl}/llms-full.txt): All pages in single file`,
		{ headers: { 'Content-Type': 'text/markdown; charset=utf-8' } },
	)
}
