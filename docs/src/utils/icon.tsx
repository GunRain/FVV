import { ImageResponse } from 'next/og'
import { readFileSync } from 'node:fs'
import sharp from 'sharp'

export const getIconImage = async (img: URL, size: { width: number; height: number }) =>
	new ImageResponse(
		<div
			style={{
				background: 'transparent',
				width: '100%',
				height: '100%',
				display: 'flex',
				alignItems: 'center',
				justifyContent: 'center'
			}}>
			<img
				src={new Uint8Array(await sharp(readFileSync(img)).png().toBuffer()).buffer as any}
				width={size.width}
				height={size.height}
				style={{ borderRadius: '4px' }}
			/>
		</div>,
		{ ...size }
	)
