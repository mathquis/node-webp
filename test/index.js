const Assert                = require('assert')
const Path					= require('path')
const File                  = require('fs').promises
const {Canvas, loadImage}	= require('canvas')
const WebP					= require('../lib/')

async function main() {
	const webpFilestat = await File.stat(Path.resolve(__dirname, './baseline.webp'))

	const img = await loadImage(Path.resolve(__dirname, './test.jpg'))

	const cv = new Canvas(img.width, img.height)
	const ctx = cv.getContext('2d')

	ctx.drawImage(img, 0, 0)

	const buffer = cv.toBuffer('raw')
	const webpBuffer = WebP.encodeBGRA(buffer, cv.width, cv.height, cv.stride, 75, false)

	Assert(webpBuffer.length == webpFilestat.size, 'Incorrect WebP image file size');

	await File.writeFile(Path.resolve(__dirname, './result.webp'), webpBuffer)
}

main()