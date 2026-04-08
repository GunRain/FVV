import { createMDX } from 'fumadocs-mdx/next'

export default createMDX()({
	assetPrefix: process.env.NODE_ENV === 'production' ? 'https://lang.fengying.xin' : undefined,
	output: process.env.NODE_ENV === 'production' ? 'export' : undefined,
	trailingSlash: true,
	reactStrictMode: true,
	rewrites: async () => [
		{
			source: '/:slug*.md',
			destination: '/llms.md/:slug*.md',
		},
	],
	images: { unoptimized: true },
})
