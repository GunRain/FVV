import fs from 'fs-extra'
import path from 'path'

const srcDir = path.join(process.cwd(), 'out/llms.md/')
const dstDir = path.join(process.cwd(), 'out/')

fs.copySync(srcDir, dstDir)
fs.removeSync(srcDir)
