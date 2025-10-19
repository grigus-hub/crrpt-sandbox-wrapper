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

	// --- declarations ---
	typedef struct JpegHandle JpegHandle;

	enum {
		JPEG_API_OK = 0,
		JPEG_API_ERR_IO = -1,
		JPEG_API_ERR_LIBJPEG = -2
	};

	CORRUPTJPEG_API JpegHandle* jpeg_open(const char* path);
	CORRUPTJPEG_API void jpeg_close(JpegHandle* handle);

#ifdef __cplusplus
}
#endif

#endif
