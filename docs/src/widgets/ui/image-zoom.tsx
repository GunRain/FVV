'use client'

import { ImageZoom as OrigImageZoom } from 'fumadocs-ui/components/image-zoom'
import Image from 'next/image'
import { useSyncExternalStore } from 'react'

export default function ImageZoom(props: any) {
	const mounted = useSyncExternalStore(
		() => () => void null,
		() => true,
		() => false
	)
	return mounted ? <OrigImageZoom {...props} /> : <Image {...props} />
}