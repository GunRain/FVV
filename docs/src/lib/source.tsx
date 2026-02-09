import {loader} from 'fumadocs-core/source'
import {lucideIconsPlugin} from 'fumadocs-core/source/lucide-icons'
import {docs} from 'fumadocs-mdx:collections/server'
import {GithubInfo} from 'fumadocs-ui/components/github-info'
import type {BaseLayoutProps} from 'fumadocs-ui/layouts/shared'
import type {Author} from 'next/dist/lib/metadata/types/metadata-types'
import Image from 'next/image'
import Link from 'next/link'
import {type ReactNode} from 'react'

export const docsConfig = {
	title: 'FVV 语言',
	baseUrl: 'https://fvvlang.sbs',
	authors: [
		{name: 'ShIroRRen', url: 'https://shiror.ren/#shiro'},
		{name: 'FengYing', url: 'https://www.fengying.xin/#ying'},
		{name: 'Linso', url: 'https://linso.top/'}
	] satisfies Author[],
	icon: {
		url: new URL('../assets/logo.svg', import.meta.url),
		size: 1314
	} satisfies {
		url: URL
		size: number | {width: number; height: number}
	},
	footer: {
		links: [
			{
				title: 'FVV 语言',
				items: [
					{
						label: 'FVV',
						href: '/spec'
					},
					{
						label: 'FWW',
						href: '/widgets'
					},
					{
						label: 'FWS',
						href: '/script'
					}
				]
			},
			{
				title: '友链',
				items: [
					{
						label: '广告',
						href: 'https://ja7.top/ad'
					},
					{
						label: 'NGA 开发文档',
						href: 'https://app.niggergo.work'
					},
					{
						label: 'Latest File',
						href: 'https://latestfile.zip'
					},
					{
						label: '世界之外工作组',
						href: 'https://oom-wg.dev'
					}
				]
			}
		],
		copyright: (
			<span>
				<Link href='https://linso.top/' target='_blank'>
					Linso
				</Link>{' '}
				牛逼
				<br />
				根据{' '}
				<Link href='https://license.fileto.download/' target='_blank'>
					F2DLPR License
				</Link>{' '}
				授权许可。©️ 2016-{new Date().getFullYear()}{' '}
				<Link href='https://shiror.ren/' target='_blank'>
					白彩恋
				</Link>
				，版权所有，保留一切权利。
			</span>
		)
	} satisfies {
		links: {
			title: string
			items: {
				label: string
				href: string
			}[]
		}[]
		copyright: ReactNode
	},
	git: {
		user: 'FVV-Lang',
		repo: 'FVV',
		branch: 'fw',
		dir: 'docs'
	} satisfies {
		user: string
		repo: string
		branch: string
		dir?: string
	} as {
		user: string
		repo: string
		branch: string
		dir?: string
	}
}

export const docsOptions = {
	nav: {
		title: (
			<div className='flex items-center gap-2'>
				<Image
					alt='Logo'
					src='/icon'
					width={32}
					height={32}
					className='rounded-md'
					priority
					unoptimized
				/>
				<span className='font-semibold'>{docsConfig.title}</span>
			</div>
		)
	},
	links: [
		{
			type: 'custom' as const,
			children: <GithubInfo owner={docsConfig.git.user} repo={docsConfig.git.repo} />
		}
	],
	githubUrl: `https://github.com/${docsConfig.git.user}/${docsConfig.git.repo}`,
	themeSwitch: {mode: 'light-dark-system' as const}
} satisfies BaseLayoutProps as BaseLayoutProps

export const source = loader({
	baseUrl: '/',
	source: docs.toFumadocsSource(),
	plugins: [lucideIconsPlugin()]
})
