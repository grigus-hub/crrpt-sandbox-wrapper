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

CORRUPTJPEG_API int jpeg_get_width(const JpegHandle* h) {
	return h ? h->width : 0;
}

CORRUPTJPEG_API int jpeg_get_height(const JpegHandle* h) {
	return h ? h->height : 0;
}

CORRUPTJPEG_API int jpeg_get_num_components(const JpegHandle* h) {
	return h ? h->num_components : 0;
}

int jpeg_get_num_blocks_x(JpegHandle* h, int component) {
    if (!h || component < 0 || component >= h->dinfo.num_components)
        return JPEG_API_ERR_LIBJPEG;

    jpeg_component_info* comp = &h->dinfo.comp_info[component];
    return (int)comp->width_in_blocks;
}

int jpeg_get_num_blocks_y(JpegHandle* h, int component) {
    if (!h || component < 0 || component >= h->dinfo.num_components)
        return JPEG_API_ERR_LIBJPEG;

    jpeg_component_info* comp = &h->dinfo.comp_info[component];
    return (int)comp->height_in_blocks;
}

static inline short clamp_coef(int v) {
    if (v < -1024) v = -1024;
    if (v > 1023) v = 1023;
    return (short)v;
}

CORRUPTJPEG_API int jpeg_get_dct_block(const JpegHandle* h, int channel, int bx, int by,
    float* out_block, size_t out_len)
{
    if (!h || !out_block || out_len < 64) return JPEG_API_ERR_INVALID_PARAM;
    if (channel < 0 || channel >= h->num_components) return JPEG_API_ERR_INVALID_PARAM;

    jpeg_component_info* comp = &h->dinfo.comp_info[channel];
    if (bx < 0 || bx >= comp->width_in_blocks || by < 0 || by >= comp->height_in_blocks)
        return JPEG_API_ERR_INVALID_PARAM;

    JBLOCKARRAY buffer = h->dinfo.mem->access_virt_barray(
        (j_common_ptr)&h->dinfo, h->coef_arrays[channel], by, 1, FALSE);

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            out_block[i * 8 + j] = (float)buffer[0][bx][i * 8 + j];
        }
    }
    return JPEG_DCT_BLOCK_SIZE;
}

CORRUPTJPEG_API int jpeg_set_dct_block(JpegHandle* h, int channel, int bx, int by,
    const float* in_block, size_t in_len)
{
    if (!h || !in_block || in_len < 64) return JPEG_API_ERR_INVALID_PARAM;
    if (channel < 0 || channel >= h->num_components) return JPEG_API_ERR_INVALID_PARAM;

    jpeg_component_info* comp = &h->dinfo.comp_info[channel];
    if (bx < 0 || bx >= comp->width_in_blocks || by < 0 || by >= comp->height_in_blocks)
        return JPEG_API_ERR_INVALID_PARAM;

    JBLOCKARRAY buffer = h->dinfo.mem->access_virt_barray(
        (j_common_ptr)&h->dinfo, h->coef_arrays[channel], by, 1, TRUE);

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            buffer[0][bx][i * 8 + j] = clamp_coef((int)in_block[i * 8 + j]);
        }
    }
    return JPEG_DCT_BLOCK_SIZE;
}