import * as AccordionComponents from 'fumadocs-ui/components/accordion'
// import * as BannerComponents from 'fumadocs-ui/components/banner'
import * as CodeBlockComponents from 'fumadocs-ui/components/codeblock'
// import * as DynamicCodeBlockComponents from 'fumadocs-ui/components/dynamic-codeblock'
import * as FilesComponents from 'fumadocs-ui/components/files'
import * as GithubInfoComponents from 'fumadocs-ui/components/github-info'
import * as ImageZoomComponents from 'fumadocs-ui/components/image-zoom'
// import * as InlineTOCComponents from 'fumadocs-ui/components/inline-toc'
import * as StepsComponents from 'fumadocs-ui/components/steps'
import * as TabsComponents from 'fumadocs-ui/components/tabs'
import * as TypeTableComponents from 'fumadocs-ui/components/type-table'
import {CodeBlock, Pre} from 'fumadocs-ui/components/codeblock'
import {ImageZoom} from 'fumadocs-ui/components/image-zoom'
import defaultMdxComponents from 'fumadocs-ui/mdx'
import type {MDXComponents} from 'mdx/types'
import Link from 'next/link'
import {type ComponentPropsWithoutRef} from 'react'

export const getMDXWidgets = (components?: MDXComponents) =>
	({
		...defaultMdxComponents,
		...AccordionComponents,
		// ...BannerComponents,
		...CodeBlockComponents,
		pre: ({children, ...props}: ComponentPropsWithoutRef<'pre'>) => (
			<CodeBlock {...props}>
				<Pre>{children}</Pre>
			</CodeBlock>
		),
		// ...DynamicCodeBlockComponents,
		...FilesComponents,
		...GithubInfoComponents,
		...ImageZoomComponents,
		img: props => <ImageZoom {...props} />,
		// ...InlineTOCComponents,
		Link,
		...StepsComponents,
		...TabsComponents,
		...TypeTableComponents,
		...components
	}) satisfies MDXComponents
