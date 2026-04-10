import { NextResponse } from 'next/server'

import { source } from '@/lib/source'
import { getLLMText } from '@/utils/llms'

export const revalidate = false

export const GET = async () =>
	new NextResponse(
		(
			await Promise.all(
				source
					.getPages()
					.filter(page => !page.data['go-import'] && !page.data['go-source'])
					.map(getLLMText)
			)
		).join('\n\n'),
		{ headers: { 'Content-Type': 'text/markdown; charset=utf-8' } }
	)
