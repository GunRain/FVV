'use client'
import {useEffect} from 'react'

import clarity from '@microsoft/clarity'

export default () => (
	useEffect(() => {
		!['127.0.0.1', 'localhost'].includes(window.location.hostname) &&
			process.env.NODE_ENV === 'production' &&
			(clarity.init('vekou24a6v'), clarity.consentV2())
		// Clarity: https://clarity.microsoft.com/
	}, []),
	null
)
