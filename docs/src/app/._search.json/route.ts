import { stopwords as englishStopwords } from '@orama/stopwords/english'
import { stopwords as mandarinStopwords } from '@orama/stopwords/mandarin'
import { createTokenizer } from '@orama/tokenizers/mandarin'
import { createFromSource } from 'fumadocs-core/search/server'

import { source } from '@/lib/source'

export const revalidate = false

export const { staticGET: GET } = createFromSource(source, {
	components: {
		tokenizer: createTokenizer({
			language: 'mandarin',
			stopWords: [...mandarinStopwords, ...englishStopwords]
		})
	}
})