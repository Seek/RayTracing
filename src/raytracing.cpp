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
#include "PerspectiveCamera.hpp"
#define IMWIDTH 1024
#define IMHEIGHT 768

std::vector<uint8_t> g_red(IMWIDTH*IMHEIGHT);
std::vector<uint8_t> g_green(IMWIDTH*IMHEIGHT);
std::vector<uint8_t> g_blue(IMWIDTH*IMHEIGHT);

static const int SamplesPerPixel = 16;

static int save_png_to_file(const char *path);
void readpng_version_info();

vec3 brdf(const vec3& wo, const vec3& wi, const vec3& color)
{
	return color * M_1_PI;
}

int main(int argc, char* argv[]) 
{

	//Samples
	RNG rng;
	std::vector<float> image_samples;
	//image_samples.reserve(IMWIDTH * IMHEIGHT * SamplesPerPixel * 2);
	//Generate image samples
	// Setup  camera
	Transform worldToCamera = Transform::lookat(vec3(0.0f, 0.0f, -400.0f), vec3(), vec3(0.0f, 1.0f, 0.0f));
	float aspectRatio = float(IMWIDTH) / float(IMHEIGHT);
	float hFOV = degreeToRadians(66.0f); //degrees
	float vFOV = 2 * atan(tan(hFOV / 2) * aspectRatio);
	float yScale = 1.0f / tan(vFOV / 2);
	float xScale = yScale / aspectRatio;
	float near = 0.1f; float far = 1000.0f;
	float right = near / xScale;
	float left = -right;
	float top = -near / yScale;
	float bottom = -top;
	std::unique_ptr<Camera> cam( new PerspectiveCamera(inv(worldToCamera),worldToCamera, left, right, top, bottom,
		near, far, IMWIDTH, IMHEIGHT));
	//readpng_version_info();
	// Setup scene
	std::vector<std::shared_ptr<Renderable> > objects;
	objects.reserve(10);
	//Add a sphere
	Transform transformSphere = Transform::translate(100.0, 0.0f, 95.0f);
	std::shared_ptr<Sphere> sphere(new Sphere(transformSphere, inv(transformSphere), 100.0f));
	std::shared_ptr<Material> sphereMat(new Material(vec3(0.0f, 0.5f, 0.5f)));
	std::shared_ptr<Plane> plane(new Plane(vec3(0.0, 0.0f, 200.0f), vec3(0.0f, 0.0f, -1.0f))); //Back wall
	std::shared_ptr<Plane> plane1(new Plane(vec3(0.0, 200.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f))); //Ceiling
	std::shared_ptr<Plane> plane2(new Plane(vec3(0.0, -200.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f))); //Floot
	std::shared_ptr<Plane> plane3(new Plane(vec3(-200.0, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f))); //Left Wall
	std::shared_ptr<Plane> plane4(new Plane(vec3(200.0, 0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f))); //Right Wall
	std::shared_ptr<Material> planeMat(new Material(vec3(1.0f, 1.0f, 1.0f)));
	objects.push_back(std::shared_ptr<Renderable>(new Renderable(sphere, sphereMat)));
	objects.push_back(std::shared_ptr<Renderable>(new Renderable(plane, planeMat)));
	objects.push_back(std::shared_ptr<Renderable>(new Renderable(plane1, planeMat)));
	objects.push_back(std::shared_ptr<Renderable>(new Renderable(plane2, planeMat)));
	objects.push_back(std::shared_ptr<Renderable>(new Renderable(plane3, planeMat)));
	objects.push_back(std::shared_ptr<Renderable>(new Renderable(plane4, planeMat)));
	std::shared_ptr<Intersection> intersection(new Intersection());
	Intersection trueIntersection;
	vec3 light(0.0, 190.0f, 0.0f);
	bool hit;
	for (int i = 0; i < IMHEIGHT; ++i)
	{
		for (int j = 0; j < IMWIDTH; ++j)
		{
			int numHit = 0;
			vec3 pras(float(j), float(i), 0.0f);
			Ray ray = cam->generateRay(float(j), float(i));
			//fprintf(stderr, "Casting ray in dir: %f,%f,%f \n", ray.dir.x, ray.dir.y, ray.dir.z);
			float tHit = std::numeric_limits<float>::infinity();
			for each( auto& surf in objects)
			{
				float tObj = 0;
				hit = surf->intersect(ray, &tObj, intersection);
				if (hit == false) continue;
				numHit += 1;
				if (tObj < tHit)
				{
					trueIntersection = *intersection;
					tHit = tObj;
				}
			}
			if (numHit == 0)
			{
				g_red[i* IMWIDTH + j] = 0;
				g_blue[i* IMWIDTH + j] = 0;
				g_green[i* IMWIDTH + j] = 0;
				continue;
			}

			//One bounce?
			//fprintf(stderr, "Number of objects hit: %i\n", numHit);
			vec3 L = normalize(light - trueIntersection.ls->p);
			float d = (light - trueIntersection.ls->p).length(); d *= d;
			float ndl = dot(trueIntersection.ls->n, L);
			float lterm = std::max(ndl, 0.0f);
			vec3 color = brdf(vec3(), vec3(), trueIntersection.material->getColor());
			float rr = color.x * lterm * (20000.0f / d);
			float gg = color.y * lterm * (20000.0f / d);
			float bb = color.z * lterm * (20000.0f / d);
			vec3 newRayDir = cosineSampleHemisphere(rng.nextFloat(), rng.nextFloat());
			Ray newRay(trueIntersection.ls->p, normalize(newRayDir), tHit + 0.00001f);
			numHit = 0;
			tHit = std::numeric_limits<float>::infinity();
			for each(auto& surf in objects)
			{
				float tObj = 0;
				hit = surf->intersect(newRay, &tObj, intersection);
				if (hit == false) continue;
				numHit += 1;
				if (tObj < tHit)
				{
					trueIntersection = *intersection;
					tHit = tObj;
				}
			}
			if (numHit == 0)
			{
				rr = std::pow(rr, 1.0f / 2.2f);
				gg = std::pow(gg, 1.0f / 2.2f);
				bb = std::pow(bb, 1.0f / 2.2f);
				rr = std::min(rr, 1.0f); gg = std::min(gg, 1.0f); bb = std::min(bb, 1.0f);
				uint8_t r = std::min(uint8_t((rr * 255.0f)), (uint8_t)255);
				uint8_t g = std::min(uint8_t((gg * 255.0f)), (uint8_t)255);
				uint8_t b = std::min(uint8_t((bb * 255.0f)), (uint8_t)255);
				g_red[i* IMWIDTH + j] = r;
				g_green[i* IMWIDTH + j] = g;
				g_blue[i * IMWIDTH + j] = b;
				continue;
			}
			L = normalize(light - trueIntersection.ls->p);
			d = (light - trueIntersection.ls->p).length(); d *= d;
			ndl = dot(trueIntersection.ls->n, L);
			lterm = std::max(ndl, 0.0f);
			color = brdf(vec3(), vec3(), trueIntersection.material->getColor());
			float dddd = (dot(vec3(0, 0, 1), normalize(newRayDir)) / cosineHemispherePdf(normalize(newRayDir).z, 0.0f));
			rr += color.x * lterm * (20000.0f / d) *  dddd;
			gg += color.y * lterm * (20000.0f / d) * dddd;
			bb += color.z * lterm * (20000.0f / d) * dddd;
			rr = std::pow(rr, 1.0f / 2.2f);
			gg = std::pow(gg, 1.0f / 2.2f);
			bb = std::pow(bb, 1.0f / 2.2f);
			rr = std::min(rr, 1.0f); gg = std::min(gg, 1.0f); bb = std::min(bb, 1.0f);
			uint8_t r = std::min(uint8_t((rr * 255.0f)), (uint8_t) 255); 
			uint8_t g = std::min(uint8_t((gg * 255.0f)), (uint8_t)255);
			uint8_t b = std::min(uint8_t((bb * 255.0f)), (uint8_t)255);
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