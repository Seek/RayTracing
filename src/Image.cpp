#include "Image.hpp"

Image::Image(int _width, int _height)
	: width(_width), height(_height)
{
	pixels.resize((width + 1) * (height + 1));
	aspectRatio = float(width) / float(height);
}

void Image::addSample(const Sample & sample, const vec3 & color)
{
	float imX = std::floor(sample.imageX); //Samples are continuous
	float imY = std::floor(sample.imageY);
	Pixel& p = pixels[int(imY) * width + int(imX)];
	p.r += color.x; p.g += color.y; p.b += color.z;
	p.weightSum += 1;
}

int Image::saveImage(const char * path)
{
	FILE * fp;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	size_t x, y;
	png_byte ** row_pointers = NULL;
	/* "status" contains the return value of this function. At first
	it is set to a value which means 'failure'. When the routine
	has finished its work, it is set to a value which means
	'success'. */
	int status = -1;
	/* The following number is set by trial and error only. I cannot
	see where it it is documented in the libpng manual.
	*/
	int pixel_size = 3;
	int depth = 8;

	fp = fopen(path, "wb");
	if (!fp) {
		goto fopen_failed;
	}

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		goto png_create_write_struct_failed;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		goto png_create_info_struct_failed;
	}

	/* Set up error handling. */

	if (setjmp(png_jmpbuf(png_ptr))) {
		goto png_failure;
	}

	/* Set image attributes. */

	png_set_IHDR(png_ptr,
		info_ptr,
		width,
		height,
		depth,
		PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);

	/* Initialize rows of PNG. */

	row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_byte *));
	for (y = 0; y < height; ++y) {
		png_byte *row =
			(png_bytep)png_malloc(png_ptr, sizeof(uint8_t) * width * pixel_size);
		row_pointers[y] = row;
		for (x = 0; x < width; ++x) {
			//Grab the current pixel
			Pixel p = pixels[y * width + x];
			//Calculate colors
			float r = p.r / p.weightSum;
			float g = p.g / p.weightSum;
			float b = p.b / p.weightSum;
			//Correct for gamma
			r = std::pow(r, 1.0f / 2.2f);
			g = std::pow(g, 1.0f / 2.2f);
			b = std::pow(b, 1.0f / 2.2f);
			//Clamp just in case
			r = std::min(r, 1.0f); 
			g = std::min(g, 1.0f); 
			b = std::min(b, 1.0f);
			//Convert to uint and write
			*row++ = png_byte(r * 255.0f);
			*row++ = png_byte(g * 255.0f);
			*row++ = png_byte(b * 255.0f);
		}
	}

	/* Write the image data to "fp". */

	png_init_io(png_ptr, fp);
	png_set_rows(png_ptr, info_ptr, row_pointers);
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	/* The routine has successfully written the file, so we set
	"status" to a value which indicates success. */

	status = 0;

	for (y = 0; y < height; y++) {
		png_free(png_ptr, row_pointers[y]);
	}
	png_free(png_ptr, row_pointers);

png_failure:
png_create_info_struct_failed:
	png_destroy_write_struct(&png_ptr, &info_ptr);
png_create_write_struct_failed:
	fclose(fp);
fopen_failed:
	return status;
}
