#ifndef CORRUPTME_JPEG_API_H
#define CORRUPTME_JPEG_API_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

	// Handler to an opened JPEG file
	typedef struct JpegHandle JpegHandle;

	// Errors
	enum {
		JPEG_API_OK = 0,
		JPEG_API_ERR_IO = -1,
		JPEG_API_ERR_LIBJPEG = -2
	};

	// Lifecycle
	JpegHandle* jpeg_open(const char* path);
	void jpeg_close(JpegHandle* handle);

#endif