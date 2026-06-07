import { NextResponse } from 'next/server'

import { docsConfig } from '@/lib/source'

export const revalidate = false

export const GET = () => new NextResponse(new URL(docsConfig.baseUrl).hostname)