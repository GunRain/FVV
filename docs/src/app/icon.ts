import {docsConfig} from '@/lib/source'
import {getIconImage} from '@/utils/icon'

export const revalidate = false

export const size =
	typeof docsConfig.icon.size === 'number'
		? {width: docsConfig.icon.size, height: docsConfig.icon.size}
		: docsConfig.icon.size
export const contentType = 'image/png'

export default () => getIconImage(docsConfig.icon.url, size)
