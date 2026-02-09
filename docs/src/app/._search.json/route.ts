import {createFromSource} from 'fumadocs-core/search/server'

import {source} from '@/lib/source'
import {stopwords as englishStopwords} from '@orama/stopwords/english'
import {stopwords as mandarinStopwords} from '@orama/stopwords/mandarin'
import {createTokenizer} from '@orama/tokenizers/mandarin'

export const revalidate = false

export const {staticGET: GET} = createFromSource(source, {
	components: {
		tokenizer: createTokenizer({
			language: 'mandarin' as const,
			stopWords: [...mandarinStopwords, ...englishStopwords]
		})
	}
})
