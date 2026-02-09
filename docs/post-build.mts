import fs from 'fs-extra'
import path from 'path'

;(async () => {
	const srcDir = path.join(process.cwd(), 'out/llms.md/')
	const dstDir = path.join(process.cwd(), 'out/')

	if (await fs.pathExists(srcDir)) {
		await fs.copy(srcDir, dstDir)
		await fs.remove(srcDir)
	}
})()
