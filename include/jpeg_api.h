#ifndef CORRUPTME_JPEG_API_H
#define CORRUPTME_JPEG_API_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#ifdef _WIN32
#ifdef CORRUPTJPEG_EXPORTS
#define CORRUPTJPEG_API extern "C" __declspec(dllexport)
#else
#define CORRUPTJPEG_API extern "C" __declspec(dllimport)
#endif
#else
#define CORRUPTJPEG_API extern "C"
#endif

// Handler to an opened JPEG file
typedef struct JpegHandle JpegHandle;

// Errors
enum {
	JPEG_API_OK = 0,
	JPEG_API_ERR_IO = -1,
	JPEG_API_ERR_LIBJPEG = -2
};

// Lifecycle
CORRUPTJPEG_API JpegHandle* jpeg_open(const char* path);
CORRUPTJPEG_API void jpeg_close(JpegHandle* handle);

#endif