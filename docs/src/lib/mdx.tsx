import * as AccordionWidgets from 'fumadocs-ui/components/accordion'
import * as CodeBlockWidgets from 'fumadocs-ui/components/codeblock'
import * as FilesWidgets from 'fumadocs-ui/components/files'
import * as StepsWidgets from 'fumadocs-ui/components/steps'
import * as TabsWidgets from 'fumadocs-ui/components/tabs'
// import {Banner} from 'fumadocs-ui/components/banner'
import {CodeBlock, Pre} from 'fumadocs-ui/components/codeblock'
// import {DynamicCodeBlock} from 'fumadocs-ui/components/dynamic-codeblock'
import {GithubInfo} from 'fumadocs-ui/components/github-info'
import {ImageZoom} from 'fumadocs-ui/components/image-zoom'
// import {InlineTOC} from 'fumadocs-ui/components/inline-toc'
import {TypeTable} from 'fumadocs-ui/components/type-table'
import defaultMdxComponents from 'fumadocs-ui/mdx'
import type {MDXComponents} from 'mdx/types'
import Image from 'next/image'
import Link from 'next/link'
import {type ComponentPropsWithoutRef} from 'react'

export const getMDXWidgets = (components?: MDXComponents) =>
	({
		...defaultMdxComponents,
		...AccordionWidgets,
		// Banner,
		...CodeBlockWidgets,
		pre: ({children, ...props}: ComponentPropsWithoutRef<'pre'>) => (
			<CodeBlock {...props}>
				<Pre>{children}</Pre>
			</CodeBlock>
		),
		// DynamicCodeBlock,
		...FilesWidgets,
		GithubInfo,
		Image,
		ImageZoom,
		img: props => <ImageZoom {...(props as any)} />,
		// InlineTOC,
		Link,
		...StepsWidgets,
		...TabsWidgets,
		TypeTable,
		...components
	}) satisfies MDXComponents
