#ifndef CORRUPTME_JPEG_API_H
#define CORRUPTME_JPEG_API_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#ifdef _WIN32
#ifdef CORRUPTJPEG_EXPORTS
#define CORRUPTJPEG_API __declspec(dllexport)
#else
#define CORRUPTJPEG_API __declspec(dllimport)
#endif
#else
#define CORRUPTJPEG_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define JPEG_DCT_BLOCK_SIZE 64

	typedef struct JpegHandle JpegHandle;

	// --- Error codes ---
	enum {
		JPEG_API_OK = 0,
		JPEG_API_ERR_IO = -1,
		JPEG_API_ERR_LIBJPEG = -2,
		JPEG_API_ERR_INVALID_PARAM = -3,
		JPEG_API_ERR_NOT_LOADED = -4
	};

	// --- Lifecycle ---
	CORRUPTJPEG_API JpegHandle* jpeg_open(const char* path);
	CORRUPTJPEG_API void jpeg_close(JpegHandle* handle);

	// --- Get image information ---
	CORRUPTJPEG_API int jpeg_get_width(const JpegHandle* h);
	CORRUPTJPEG_API int jpeg_get_height(const JpegHandle* h);
	CORRUPTJPEG_API int jpeg_get_num_components(const JpegHandle* h);

	// --- Blocks in component ---
	CORRUPTJPEG_API int jpeg_get_num_blocks_x(JpegHandle* handle, int component);
	CORRUPTJPEG_API int jpeg_get_num_blocks_y(JpegHandle* handle, int component);

	// --- DCT coeffs ---
	CORRUPTJPEG_API int jpeg_get_dct_block(
		const JpegHandle* h, int channel, int block_x, int block_y,
		float* out_block, size_t out_len
	);

	CORRUPTJPEG_API int jpeg_set_dct_block(
		JpegHandle* h, int channel, int block_x, int block_y,
		const float* in_block, size_t in_len
	);

#ifdef __cplusplus
}
#endif

#endif
