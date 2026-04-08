'use client'
import { useDocsSearch } from 'fumadocs-core/search/client'
import {
	SearchDialog,
	SearchDialogClose,
	SearchDialogContent,
	SearchDialogHeader,
	SearchDialogIcon,
	SearchDialogInput,
	SearchDialogList,
	SearchDialogOverlay,
	type SharedProps,
} from 'fumadocs-ui/components/dialog/search'

import { create } from '@orama/orama'
import { stopwords as englishStopwords } from '@orama/stopwords/english'
import { stopwords as mandarinStopwords } from '@orama/stopwords/mandarin'
import { createTokenizer } from '@orama/tokenizers/mandarin'

const initOrama = (_loc?: string) =>
	create({
		schema: { _: 'string' },
		components: {
			tokenizer: createTokenizer({
				language: 'mandarin',
				stopWords: [...mandarinStopwords, ...englishStopwords],
				stemmer: word => word.toLowerCase(),
			}),
		},
	})

export default (props: SharedProps) => {
	const { search, setSearch, query } = useDocsSearch({
		type: 'static',
		initOrama,
		from: '/._search.json',
	})
	return (
		<SearchDialog search={search} onSearchChange={setSearch} isLoading={query.isLoading} {...props}>
			<SearchDialogOverlay />
			<SearchDialogContent>
				<SearchDialogHeader>
					<SearchDialogIcon />
					<SearchDialogInput />
					<SearchDialogClose />
				</SearchDialogHeader>
				<SearchDialogList items={query.data !== 'empty' ? query.data : null} />
			</SearchDialogContent>
		</SearchDialog>
	)
}
