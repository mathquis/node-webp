#include <napi.h>
#include <stdint.h>
#include <encode.h>
#include <iostream>

using namespace Napi;

struct EncodeOptions
{
	const uint8_t* data;
	unsigned int width;
	unsigned int height;
	unsigned int stride;
	float quality = 75.0;
	bool lossless = false;
};

EncodeOptions parseOptions(const CallbackInfo& args)
{
	Env env = args.Env();

	struct EncodeOptions opts;

	opts.data = args[0].As<TypedArrayOf<uint8_t>>().Data();

	if (!args[1].IsNumber()) {
		throw Napi::Error::New(env, "Parameter \"width\" must be a number");
	}

	opts.width = args[1].As<Number>().Uint32Value();

	if (opts.width <= 0) {
		throw Napi::Error::New(env, "Parameter \"width\" must be a greater than 0");
	}

	if (!args[2].IsNumber()) {
		throw Napi::Error::New(env, "Parameter \"height\" must be a number");
	}

	opts.height	= args[2].As<Number>().Uint32Value();

	if (opts.height <= 0) {
		throw Napi::Error::New(env, "Parameter \"height\" must be a greater than 0");
	}

	if (!args[3].IsNumber()) {
		throw Napi::Error::New(env, "Parameter \"stride\" must be a number");
	}

	opts.stride	= args[3].As<Number>().Uint32Value();

	if (!args[4].IsNumber()) {
		throw Napi::Error::New(env, "Parameter \"quality\" must be a number");
	}

	opts.quality = args[4].As<Number>().FloatValue();

	if (opts.quality < 0.0 || opts.quality > 100.0) {
		throw Napi::Error::New(env, "Parameter \"quality\" must be a float between 0.0 and 100.0");
	}

	if (!args[5].IsBoolean()) {
		throw Napi::Error::New(env, "Parameter \"lossless\" must be a boolean");
	}

	opts.lossless = args[5].As<Boolean>().Value();

	return opts;
}

void free_webp(Env env, void* data) {
  WebPFree(data);
}

Value EncodeBGRA(const CallbackInfo& args)
{
	Env env = args.Env();

	EncodeOptions opts;
	try {
		opts = parseOptions(args);
	} catch (const Error& e) {
		Error::New(args.Env(), e.what()).ThrowAsJavaScriptException();
		return Value();
	}

	uint8_t* out = NULL;
  	size_t outSize;

	if (opts.lossless) {
	  outSize = WebPEncodeLosslessBGRA(opts.data, opts.width, opts.height, opts.stride, &out);
	} else {
	  outSize = WebPEncodeBGRA(opts.data, opts.width, opts.height, opts.stride, opts.quality, &out);
	}

	if (outSize == 0) {
		Error::New(args.Env(), "Encoding error").ThrowAsJavaScriptException();
		return Value();
	}

	return Buffer<uint8_t>::New(env, out, outSize, free_webp);
}

Value EncodeBGR(const CallbackInfo& args)
{
	Env env = args.Env();

	EncodeOptions opts;
	try {
		opts = parseOptions(args);
	} catch (const Error& e) {
		Error::New(args.Env(), e.what()).ThrowAsJavaScriptException();
		return Value();
	}

	uint8_t* out = NULL;
  	size_t outSize;

	if (opts.lossless) {
	  outSize = WebPEncodeLosslessBGR(opts.data, opts.width, opts.height, opts.stride, &out);
	} else {
	  outSize = WebPEncodeBGR(opts.data, opts.width, opts.height, opts.stride, opts.quality, &out);
	}

	if (outSize == 0) {
		Error::New(args.Env(), "Encoding error").ThrowAsJavaScriptException();
		return Value();
	}

	return Buffer<uint8_t>::New(env, out, outSize, free_webp);
}

Value EncodeRGBA(const CallbackInfo& args)
{
	Env env = args.Env();

	EncodeOptions opts;
	try {
		opts = parseOptions(args);
	} catch (const Error& e) {
		Error::New(args.Env(), e.what()).ThrowAsJavaScriptException();
		return Value();
	}

	uint8_t* out = NULL;
  	size_t outSize;

	if (opts.lossless) {
	  outSize = WebPEncodeLosslessRGBA(opts.data, opts.width, opts.height, opts.stride, &out);
	} else {
	  outSize = WebPEncodeRGBA(opts.data, opts.width, opts.height, opts.stride, opts.quality, &out);
	}

	if (outSize == 0) {
		Error::New(args.Env(), "Encoding error").ThrowAsJavaScriptException();
		return Value();
	}

	std::cout << outSize << std::endl;

	return Buffer<uint8_t>::New(env, out, outSize, free_webp);
}

Value EncodeRGB(const CallbackInfo& args)
{
	Env env = args.Env();

	EncodeOptions opts;
	try {
		opts = parseOptions(args);
	} catch (const Error& e) {
		Error::New(args.Env(), e.what()).ThrowAsJavaScriptException();
		return Value();
	}

	uint8_t* out = NULL;
  	size_t outSize;

	if (opts.lossless) {
	  outSize = WebPEncodeLosslessRGB(opts.data, opts.width, opts.height, opts.stride, &out);
	} else {
	  outSize = WebPEncodeRGB(opts.data, opts.width, opts.height, opts.stride, opts.quality, &out);
	}

	if (outSize == 0) {
		Error::New(args.Env(), "Encoding error").ThrowAsJavaScriptException();
		return Value();
	}

	return Buffer<uint8_t>::New(env, out, outSize, free_webp);
}

/* ============================================
 Native module initialization
============================================ */

Object Init(Env env, Object exports) {
	exports.Set(String::New(env, "encodeBGRA"), Function::New(env, EncodeBGRA));
	exports.Set(String::New(env, "encodeBGR"), Function::New(env, EncodeBGR));
	exports.Set(String::New(env, "encodeRGBA"), Function::New(env, EncodeRGBA));
	exports.Set(String::New(env, "encodeRGB"), Function::New(env, EncodeRGB));

	return exports;
};

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)