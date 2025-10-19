#include "jpeg_api.h"
#include <jpeglib.h>
#include <stdlib.h>

struct JpegHandle {
	struct jpeg_decompress_struct dinfo;
	struct jpeg_error_mgr jerr;
	jvirt_barray_ptr* coef_arrays;
	FILE* infile;
	int width, height, num_components;
};

CORRUPTJPEG_API JpegHandle* jpeg_open(const char* path) {
	if (!path) return NULL;
	FILE* f = fopen(path, "rb");
	if (!f) return NULL;

	JpegHandle* h = (JpegHandle*)calloc(1, sizeof(JpegHandle));
	if (!h) { fclose(f); return NULL; }

	h->dinfo.err = jpeg_std_error(&h->jerr);
	jpeg_create_decompress(&h->dinfo);
	jpeg_stdio_src(&h->dinfo, f);
	jpeg_read_header(&h->dinfo, TRUE);
	h->coef_arrays = jpeg_read_coefficients(&h->dinfo);

	h->width = h->dinfo.image_width;
	h->height = h->dinfo.image_height;
	h->num_components = h->dinfo.num_components;
	h->infile = f;
	return h;
}

CORRUPTJPEG_API void jpeg_close(JpegHandle* h) {
	if (!h) return;
	jpeg_destroy_decompress(&h->dinfo);
	if (h->infile) fclose(h->infile);
	free(h);
}