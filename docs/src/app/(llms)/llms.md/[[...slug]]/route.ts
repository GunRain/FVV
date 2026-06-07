import { notFound } from 'next/navigation'
import { NextRequest, NextResponse } from 'next/server'

import { docsConfig, source } from '@/lib/source'
import { getLLMText } from '@/utils/llms'

export const revalidate = false

export const GET = async (_req: NextRequest, { params }: RouteContext<'/llms.md/[[...slug]]'>) => {
	let { slug } = await params
	if (!slug || slug.length === 0) notFound()
	if (slug.length !== 1 || slug[0] !== 'index.md') slug[slug.length - 1] = slug[slug.length - 1]!.replace(/\.md$/, '')
	else slug = []

	const page = source.getPage(slug)
	if (!page || page.data['go-import'] || page.data['go-source']) notFound()

	return new NextResponse(
		(await getLLMText(page)) +
			`\n---\n\n> [**Page Index**] ${'<'}${docsConfig.baseUrl}/llms.txt> | [**Full Content**] ${'<'}${docsConfig.baseUrl}/llms-full.txt>`,
		{ headers: { 'Content-Type': 'text/markdown; charset=utf-8' } }
	)
}

export const generateStaticParams = () =>
	source.getPages().map(page => ({
		slug: [
			...(page.slugs.length === 0 ? ['index.md'] : [...page.slugs.slice(0, -1), `${page.slugs[page.slugs.length - 1]}.md`])
		]
	}))