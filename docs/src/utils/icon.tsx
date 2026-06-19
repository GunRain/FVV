import { readFileSync } from 'node:fs'

import sharp from 'sharp'

export const getIconImage = async (img: URL, size: { width: number; height: number }) =>
	new Response(
		new Uint8Array(await sharp(readFileSync(img)).resize(size.width, size.height, { fit: 'contain' }).png().toBuffer()).buffer,
		{ headers: { 'Content-Type': 'image/png' } }
	)