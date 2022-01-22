# WebP

## Installation

`npm i @mathquis/node-webp`

## Usage

### Encode

```javascript
const WebP = require('@mathquis/node-webp');

const bgra = new Uint8Array(buffer)
const width = 100
const height = 100
const stride = 400
const quality = 75.0
const lossless = false

const buffer = WebP.encodeBGRA(bgra, width, height, stride, quality, lossless)
```