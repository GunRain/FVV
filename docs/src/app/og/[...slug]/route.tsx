import {generate as DefaultImage} from 'fumadocs-ui/og'
import {notFound} from 'next/navigation'
import {ImageResponse} from 'next/og'
import {NextRequest} from 'next/server'

import {docsConfig, source} from '@/lib/source'
import {getPageImage} from '@/utils/img'

export const revalidate = false

export const GET = async (_req: NextRequest, {params}: RouteContext<'/og/[...slug]'>) =>
	(page =>
		new ImageResponse(
			<DefaultImage title={page.data.title} description={page.data.description} site={docsConfig.title} />,
			{width: 1200, height: 630}
		))(source.getPage((await params).slug.slice(0, -1)) ?? notFound())

export const generateStaticParams = () =>
	source.getPages().map(page => ({
		lang: page.locale,
		slug: getPageImage(page).segments
	}))
