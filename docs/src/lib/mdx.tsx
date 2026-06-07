import * as AccordionWidgets from 'fumadocs-ui/components/accordion'
// import {Banner} from 'fumadocs-ui/components/banner'
import * as CodeBlockWidgets from 'fumadocs-ui/components/codeblock'
import { CodeBlock, Pre } from 'fumadocs-ui/components/codeblock'
// import {DynamicCodeBlock} from 'fumadocs-ui/components/dynamic-codeblock'
import * as FilesWidgets from 'fumadocs-ui/components/files'
import { GithubInfo } from 'fumadocs-ui/components/github-info'
// import {InlineTOC} from 'fumadocs-ui/components/inline-toc'
import * as StepsWidgets from 'fumadocs-ui/components/steps'
import * as TabsWidgets from 'fumadocs-ui/components/tabs'
import { TypeTable } from 'fumadocs-ui/components/type-table'
import defaultMdxComponents from 'fumadocs-ui/mdx'
import type { MDXComponents } from 'mdx/types'
import Image from 'next/image'
import Link from 'next/link'
import { type ComponentPropsWithoutRef } from 'react'

import ImageZoom from '@/widgets/ui/image-zoom'

export const getMDXWidgets = (components?: MDXComponents) =>
	({
		...defaultMdxComponents,
		...AccordionWidgets,
		// Banner,
		...CodeBlockWidgets,
		pre: ({ children, ...props }: ComponentPropsWithoutRef<'pre'>) => (
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