import { DocsBody, DocsDescription, DocsPage, DocsTitle, PageLastUpdate } from 'fumadocs-ui/layouts/docs/page'
import { createRelativeLink } from 'fumadocs-ui/mdx'
import { Bot, ExternalLink } from 'lucide-react'
import type { Metadata } from 'next'
import Link from 'next/link'
import { notFound } from 'next/navigation'

import { getMDXWidgets } from '@/lib/mdx'
import { docsConfig, source } from '@/lib/source'
import { getPageImage } from '@/utils/img'
import { LLMCopyButton, ViewOptions } from '@/widgets/ui/ai-buttons'

const BUILD_TIME = new Date().toLocaleString('zh-Hant', {
	timeZone: 'Asia/Shanghai',
	year: 'numeric',
	month: '2-digit',
	day: '2-digit'
})

export default async (props: PageProps<'/[[...slug]]'>) => {
	const page = source.getPage((await props.params).slug) ?? notFound()

	return (
		<DocsPage
			toc={page.data.toc}
			full={page.data.full}
			footer={{
				enabled: true,
				component: (
					<footer className='text-fd-muted-foreground mt-16'>
						<hr />
						{page.data.lastModified && (
							<div className='border-b-0 px-6 pt-8 text-xs italic'>
								<PageLastUpdate date={page.data.lastModified} />
							</div>
						)}
						<div className='flex flex-wrap gap-x-10 gap-y-12 px-6 py-12 text-sm'>
							{[
								...docsConfig.footer.links,
								{
									title: 'LLMs',
									items: [
										{
											label: 'llms.txt',
											href: '/llms.txt'
										},
										{
											label: 'llms-full.txt',
											href: '/llms-full.txt'
										}
									]
								}
							].map(group => (
								<div key={group.title} className='flex min-w-37.5 flex-1 flex-col gap-3'>
									<h4 className='text-fd-foreground font-semibold'>{group.title}</h4>
									<ul className='space-y-2'>
										{group.items.map(item => {
											const llmsLink = /^\/llms(-full)?\.txt$/.test(item.href)
											const extLink = /^https?:\/\//.test(item.href) || item.href.startsWith('//')
											return (
												<li key={item.label}>
													<Link
														href={item.href}
														target={extLink || llmsLink ? '_blank' : undefined}
														prefetch={llmsLink ? false : 'auto'}
														className='group hover:text-fd-primary text-sm leading-snug wrap-break-word transition-colors'>
														{item.label}
														{(extLink || llmsLink) && (
															<span className='ml-1 inline-block opacity-66 transition-opacity group-hover:opacity-100'>
																{extLink ? (
																	<ExternalLink className='size-3.5 align-text-bottom' />
																) : (
																	<Bot className='size-3.5 align-text-bottom' />
																)}
															</span>
														)}
													</Link>
												</li>
											)
										})}
									</ul>
								</div>
							))}
						</div>
						<hr />
						<div className='flex flex-col items-center justify-between gap-4 px-6 py-8 text-xs md:flex-row'>
							<p>{docsConfig.footer.copyright}</p>
							<p className='opacity-70'>
								Build Time: <span className='font-mono'>{BUILD_TIME}</span>
							</p>
						</div>
					</footer>
				)
			}}>
			<DocsTitle>{page.data.title}</DocsTitle>
			<DocsDescription className='mb-0'>{page.data.description}</DocsDescription>
			{!page.data['go-import'] && !page.data['go-source'] && (
				<div className='flex flex-row items-center gap-2 border-b pb-6'>
					<LLMCopyButton markdownUrl={`${page.url === '/' ? '/index' : page.url}.md`} />
					<ViewOptions
						markdownUrl={`${page.url === '/' ? '/index' : page.url}.md`}
						githubUrl={`https://github.com/${docsConfig.git.user}/${docsConfig.git.repo}/blob/${docsConfig.git.branch}/${docsConfig.git.dir ? `${docsConfig.git.dir}/` : ''}content/${page.path}`}
					/>
				</div>
			)}
			<DocsBody>
				<page.data.body components={getMDXWidgets({ a: createRelativeLink(source, page) })} />
			</DocsBody>
		</DocsPage>
	)
}

export const generateStaticParams = () => source.generateParams()

export const generateMetadata = async (props: PageProps<'/[[...slug]]'>) => {
	const page = source.getPage((await props.params).slug) ?? notFound()

	const isGoPkg = page.data['go-import'] || page.data['go-source']
	const isIndex = page.url === '/'

	const titleConfig = isIndex ? { absolute: docsConfig.title } : isGoPkg ? { absolute: page.data.title } : page.data.title

	return {
		alternates: { canonical: page.url || '/' },
		title: titleConfig,
		description: page.data.description,
		keywords: page.data.keywords,
		openGraph: {
			title: titleConfig,
			description: page.data.description,
			url: `${docsConfig.baseUrl}${page.url}`,
			images: getPageImage(page).url
		},
		twitter: {
			title: titleConfig,
			description: page.data.description,
			images: getPageImage(page).url
		},
		other: {
			...(page.data['go-import'] && { 'go-import': page.data['go-import'] }),
			...(page.data['go-source'] && { 'go-source': page.data['go-source'] })
		},
		...(isGoPkg && {
			robots: {
				index: false,
				follow: false,
				googleBot: {
					index: false,
					follow: false
				}
			}
		})
	} satisfies Metadata
}