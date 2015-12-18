#include "vec3.hpp"
#include "mat4.hpp"
#include "Transform.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include <vector>
#include "zlib.h"
#include "png.h"
#include <stdint.h>
#include <memory>
#include "Sampling.hpp"
#include "Renderable.hpp"
#define IMWIDTH 1024
#define IMHEIGHT 768

float force;
std::vector<uint8_t> g_red(IMWIDTH*IMHEIGHT);
std::vector<uint8_t> g_green(IMWIDTH*IMHEIGHT);
std::vector<uint8_t> g_blue(IMWIDTH*IMHEIGHT);
uint8_t sphere_color[3] = { 0, 128, 128 };
uint8_t plane_color[3] = { 128, 128, 128 };

static int save_png_to_file(const char *path);
void readpng_version_info();

int main(int argc, char* argv[]) 
{
	RNG rng;
	std::vector<float> samples, samples2;
	samples.reserve(16); samples2.reserve(16 * 16 * 2);
	stratified1D(16, false, samples, rng);
	stratified2D(16, 16, true, samples2, rng);
	Transform worldToCamera = Transform::lookat(vec3(0.0f, 0.0f, -100.0f), vec3(), vec3(0.0f, 1.0f, 0.0f));
	Transform camToScreen = Transform::orthographic(float(-IMWIDTH / 2.0f), float(IMWIDTH / 2.0f), float(-IMHEIGHT / 2.0f), float(IMHEIGHT / 2.0f), 0.1f, 1000.0f);
	Transform screenToRast = Transform::screenToRaster(IMWIDTH, IMHEIGHT);
	Transform rastToScreen = inv(screenToRast);
	Transform screenToCamera = inv(camToScreen);
	Transform cameraToWorld = inv(worldToCamera);
	readpng_version_info();
	Transform t;
	Transform tinv = inv(t);
	std::vector<std::unique_ptr<Surface> > surfaces;
	surfaces.push_back(std::unique_ptr<Surface>(new Sphere(t, tinv, 125.0f)));
	//surfaces.push_back(std::unique_ptr<Surface>(new Plane(vec3(), vec3(0.0f, 1.0f, 0.0f))));
	surfaces.push_back(std::unique_ptr<Surface>(new Plane(vec3(0, 0, 150.0f), vec3(0.0f, 0.0f, -1.0f))));
	//surfaces.push_back(std::unique_ptr<Surface>(new Plane(vec3(0, 150.0, 0.0f), vec3(0.0f, -1.0f, 0.0f))));
	//surfaces.push_back(std::unique_ptr<Surface>(new Plane(vec3(0, 300.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f))));
	//surfaces.push_back(std::unique_ptr<Surface>(new Plane(vec3(0, -300.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f))));
	std::unique_ptr<LocalSurface> surfaceHit;
	LocalSurface ls;
	vec3 light(0.0f, 200.0f, 0.0f);
	bool hit;
	for (int i = 0; i < IMHEIGHT; ++i)
	{
		for (int j = 0; j < IMWIDTH; ++j)
		{
			int numHit = 0;
			vec3 pras(float(j), float(i), 0.0f);
			vec3 pcam = rastToScreen.transformPoint(pras);
			pcam = screenToCamera.transformPoint(pcam);
			Ray ray(pcam, vec3(0.0f, 0.0f, 1.0f));
			ray = cameraToWorld.transformRay(ray);
			//fprintf(stderr, "World Position at x=%f y=%f\n", ray.o.x, ray.o.y);
			ray.dir = normalize(ray.dir);
			float tHit = 0;

			for each( auto& surf in surfaces)
			{
				float tObj = 0;
				hit = surf->intersect(ray, &tObj, &ls);
				if (tObj < tHit || hit == false) continue;
				tHit = tObj;
				surfaceHit = std::unique_ptr<LocalSurface> (new LocalSurface(ls));
				numHit += 1;
			}
			if (numHit == 0)
			{
				g_red[i* IMWIDTH + j] = 0;
				g_blue[i* IMWIDTH + j] = 0;
				g_green[i* IMWIDTH + j] = 0;
				continue;
			}

			vec3 L = normalize(light - surfaceHit->p);
			float ndl = dot(surfaceHit->n, L);
			float lterm = std::max(ndl, 0.0f);
			uint8_t r = sphere_color[0] * lterm; 
			uint8_t g = sphere_color[1] * lterm;
			uint8_t b = sphere_color[2] * lterm;
			g_red[i* IMWIDTH + j] = r;
			g_green[i* IMWIDTH + j] = g;
			g_blue[i * IMWIDTH + j] = b;
		}
	}
	save_png_to_file("render.png");
}

int save_png_to_file(const char *path)
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
		IMWIDTH,
		IMHEIGHT,
		depth,
		PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);

	/* Initialize rows of PNG. */

	row_pointers = (png_bytepp)png_malloc(png_ptr, IMHEIGHT * sizeof(png_byte *));
	for (y = 0; y < IMHEIGHT; ++y) {
		png_byte *row =
			(png_bytep)png_malloc(png_ptr, sizeof(uint8_t) * IMWIDTH * pixel_size);
		row_pointers[y] = row;
		for (x = 0; x < IMWIDTH; ++x) {
			*row++ = g_red[y * IMWIDTH + x];
			*row++ = g_green[y * IMWIDTH + x];
			*row++ = g_blue[y * IMWIDTH + x];
		}
	}

	/* Write the image data to "fp". */

	png_init_io(png_ptr, fp);
	png_set_rows(png_ptr, info_ptr, row_pointers);
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	/* The routine has successfully written the file, so we set
	"status" to a value which indicates success. */

	status = 0;

	for (y = 0; y < IMHEIGHT; y++) {
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

void readpng_version_info()
{
	fprintf(stderr, "   Compiled with libpng %s; using libpng %s.\n",
		PNG_LIBPNG_VER_STRING, png_libpng_ver);
	fprintf(stderr, "   Compiled with zlib %s; using zlib %s.\n",
		ZLIB_VERSION, zlib_version);
}