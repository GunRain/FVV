import { DocsLayout } from 'fumadocs-ui/layouts/docs'
import { RootProvider } from 'fumadocs-ui/provider/next'
import { type Metadata } from 'next'
import { Ubuntu_Sans } from 'next/font/google'

import { docsConfig, docsOptions, source } from '@/lib/source'
import SearchDialog from '@/widgets/ui/search'

import './global.css'

const ubuntu = Ubuntu_Sans({ weight: ['300', '400', '500', '700'] })

export default ({ children }: LayoutProps<'/'>) => (
	<html lang='zh-Hans' className={ubuntu.className} suppressHydrationWarning>
		<head>{process.env.NODE_ENV === 'production' && <script defer src='https://☁️.ja7.top/🍥.js?ms=vekou24a6v' />}</head>
		<body className='flex min-h-screen flex-col'>
			<RootProvider
				search={{ SearchDialog }}
				i18n={{
					locale: 'zh-Hans',
					translations: {
						search: '搜索一下',
						searchNoResult: '没搜到喵...',
						toc: '在此页面...',
						lastUpdate: '最后喵喵于：'
					}
				}}>
				<DocsLayout tree={source.getPageTree()} {...docsOptions}>
					{children}
				</DocsLayout>
			</RootProvider>
		</body>
	</html>
)

export const metadata = {
	metadataBase: docsConfig.baseUrl,
	applicationName: docsConfig.title,
	title: {
		template: `%s | ${docsConfig.title}`,
		default: docsConfig.title
	},
	authors: docsConfig.authors,
	creator: docsConfig.authors[0]!.name,
	publisher: docsConfig.authors[0]!.name,
	openGraph: {
		title: {
			template: `%s | ${docsConfig.title}`,
			default: docsConfig.title
		},
		siteName: docsConfig.title,
		type: 'website'
	},
	twitter: {
		title: {
			template: `%s | ${docsConfig.title}`,
			default: docsConfig.title
		},
		card: 'summary_large_image'
	},
	robots: {
		index: true,
		follow: true,
		googleBot: {
			index: true,
			follow: true
		}
	},
	verification: {
		other: { 'baidu-site-verification': 'codeva-ZmGemFzd4y' }
	}
} satisfies Metadata