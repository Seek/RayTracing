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
#include "Image.hpp"
#include "Sample.hpp"
#include <thread>
#include <atomic>

static const int NumXSamples = 8;
static const int NumYSamples = 8;
static const int SamplesPerPixel = NumXSamples * NumYSamples;
static const int ImageWidth = 800;
static const int ImageHeight = 600;
static const int maxDepth = 3;

static int save_png_to_file(const char *path);
void readpng_version_info();
void generate_samples(std::vector<Sample>& samples, const RNG& rng);
std::atomic_uint counter;
vec3 brdf(const vec3& wo, const vec3& wi, const vec3& color)
{
	return color * float(M_1_PI);
}

void render_func(unsigned int start, unsigned int end,
	const std::vector<Sample>& samples,
	const std::vector<std::shared_ptr<Renderable> > objects,
	const std::unique_ptr<Camera>& cam, Image& render);

int main(int argc, char* argv[]) 
{
	//Create the image
	Image render(ImageWidth, ImageHeight);
	//Samples
	RNG rng;
	std::vector<Sample> image_samples((ImageWidth + 1) * (ImageHeight + 1) * SamplesPerPixel);
	//Generate image samples
	generate_samples(image_samples, rng);
#pragma region WorldSetup 
	// Setup  camera
	Transform worldToCamera = Transform::lookat(vec3(0.0f, 0.0f, -200.0f), vec3(), vec3(0.0f, 1.0f, 0.0f));
	float aspectRatio = render.getAspectRatio();
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
		near, far, float(ImageWidth), float(ImageHeight)));
	// Setup scene
	std::vector<std::shared_ptr<Renderable> > objects;
	objects.reserve(10);
	//Add a sphere
	Transform transformSphere = Transform::translate(100.0, -100.0f, 150.0f);
	std::shared_ptr<Sphere> sphere(new Sphere(transformSphere, inv(transformSphere), 100.0f));
	std::shared_ptr<Material> whiteMat(new Material(vec3(1.0f, 1.0f, 1.0f)));
	std::shared_ptr<Plane> plane(new Plane(vec3(0.0, 0.0f, 200.0f), vec3(0.0f, 0.0f, -1.0f))); //Back wall
	std::shared_ptr<Plane> plane1(new Plane(vec3(0.0, 200.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f))); //Ceiling
	std::shared_ptr<Plane> plane2(new Plane(vec3(0.0, -200.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f))); //Floot
	std::shared_ptr<Plane> plane3(new Plane(vec3(-200.0, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f))); //Left Wall
	std::shared_ptr<Plane> plane4(new Plane(vec3(200.0, 0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f))); //Right Wall
	std::shared_ptr<Material> blueMat(new Material(vec3(0.0f, 0.0f, 1.0f)));
	std::shared_ptr<Material> redMat(new Material(vec3(1.0f, 0.0f, 0.0f)));
	std::shared_ptr<Material> greenMat(new Material(vec3(0.0f, 1.0f, 0.0f)));
	objects.push_back(std::shared_ptr<Renderable>(new Renderable(sphere, whiteMat)));
	objects.push_back(std::shared_ptr<Renderable>(new Renderable(plane, whiteMat)));
	objects.push_back(std::shared_ptr<Renderable>(new Renderable(plane1, whiteMat)));
	objects.push_back(std::shared_ptr<Renderable>(new Renderable(plane2, whiteMat)));
	objects.push_back(std::shared_ptr<Renderable>(new Renderable(plane3, redMat)));
	objects.push_back(std::shared_ptr<Renderable>(new Renderable(plane4, greenMat)));
#pragma endregion
	static const int numThreads = 8;
	std::vector<std::thread> threads(numThreads);
	unsigned int numSamples = image_samples.size();
	unsigned int step = std::floor(float(numSamples) / float(numThreads));
	for (int i = 0; i < numThreads; ++i)
	{
		threads[i] = std::thread(render_func, step * i, ((step * (i + 1))),
			std::ref(image_samples), std::ref(objects), std::ref(cam), std::ref(render));
	}
	for (int i = 0; i < numThreads; ++i)
	{
		threads[i].join();
	}

	render.saveImage("render.png");
}

void readpng_version_info()
{
	fprintf(stderr, "   Compiled with libpng %s; using libpng %s.\n",
		PNG_LIBPNG_VER_STRING, png_libpng_ver);
	fprintf(stderr, "   Compiled with zlib %s; using zlib %s.\n",
		ZLIB_VERSION, zlib_version);
}

void gen_samples_thread(unsigned int start, unsigned int stop,
	std::vector<Sample>& samples, const RNG& rng)
{
	std::vector<float> rawSamples;
	rawSamples.reserve(SamplesPerPixel * 2);
	for (unsigned int i = start; i < stop;  i += SamplesPerPixel)
	{
		int y = std::floor( (double(i)/double(SamplesPerPixel)) / double(ImageWidth));
		int x = i/SamplesPerPixel - (y * ImageWidth);
		stratified2D(NumXSamples, NumYSamples, false, rawSamples, rng);
		for (int k = 0; k < SamplesPerPixel; ++k)
		{
			float tmpx = rawSamples[k * 2];
			float tmpy = rawSamples[k * 2 + 1];
			tmpx += x;
			tmpy += y;
			samples[i + k] = Sample(tmpx, tmpy);
		}
		rawSamples.clear();
	}
}
void generate_samples(std::vector<Sample>& samples, const RNG& rng)
{
	const int numThreads = 4;
	std::vector<std::thread> threads(numThreads);
	unsigned int numSamples = samples.size();
	unsigned int step = std::floor(float(numSamples) / float(numThreads));
	for (int i = 0; i < numThreads; ++i)
	{
		threads[i] = std::thread(gen_samples_thread, step * i, ((step * (i + 1))),
			std::ref(samples), std::ref(rng));
	}
	for (int i = 0; i < numThreads; ++i)
	{
		threads[i].join();
	}
}

void render_func(unsigned int start, unsigned int end,
	const std::vector<Sample>& samples, 
	 const std::vector<std::shared_ptr<Renderable> > objects,
	const std::unique_ptr<Camera>& cam, Image& render)
{
	RNG rng;
	std::shared_ptr<Intersection> intersection(new Intersection());
	Intersection trueIntersection;
	vec3 light(0.0, 190.0f, 100.0f);
	bool hit;
	float lightIntensity = 10000.0f;
	unsigned int tnSamples = samples.size();
	for (unsigned int i = start; i < end; ++i)
	{
		const Sample& sample = samples[i];
		int numHit = 0;
		Ray ray = cam->generateRay(sample.imageX, sample.imageY);
		//fprintf(stderr, "Casting ray in dir: %f,%f,%f \n", ray.dir.x, ray.dir.y, ray.dir.z);
		float tHit = std::numeric_limits<float>::infinity();
		for each(auto& surf in objects)
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
			//imag
			render.addSample(sample, vec3());
			continue;
		}

		//One bounce?
		//fprintf(stderr, "Number of objects hit: %i\n", numHit);
		vec3 L = normalize(light - trueIntersection.ls->p);
		float d = (light - trueIntersection.ls->p).length(); d *= d;
		float ndl = dot(trueIntersection.ls->n, L);
		float lterm = std::max(ndl, 0.0f);
		vec3 color = brdf(vec3(), vec3(), trueIntersection.material->getColor());
		float rr = color.x * lterm * (lightIntensity / d);
		float gg = color.y * lterm * (lightIntensity / d);
		float bb = color.z * lterm * (lightIntensity / d);
		vec3 dddd(1.0, 1.0f, 1.0f);
		vec3 oldNorm = trueIntersection.ls->n;
		vec3 oldColor = color;
		for (int k = 0; k < maxDepth; ++k)
		{
			vec3 newRayDir = normalize(cosineSampleHemisphere(rng.nextFloat(), rng.nextFloat()));
			float pdf = cosineHemispherePdf(abs(newRayDir.z), 0.0f);
			vec3 n = trueIntersection.ls->n;
			vec3 dpdu = trueIntersection.ls->dpdu;
			vec3 last = normalize(cross(n, dpdu));
			Transform world2Shading = Transform::worldToShading(n, dpdu, last);
			Transform shading2World = inv(world2Shading);
			newRayDir = shading2World.transformVector(newRayDir);
			Ray newRay(trueIntersection.ls->p, newRayDir, tHit + 0.0001f);
			float v = abs(dot(oldNorm, newRayDir));
			dddd.x *= v * oldColor.x / pdf;
			dddd.y *= v * oldColor.y / pdf;
			dddd.z *= v * oldColor.z / pdf;
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
				break;
			}
			L = normalize(light - trueIntersection.ls->p);
			d = (light - trueIntersection.ls->p).length(); d *= d;
			vec3 norm = trueIntersection.ls->n;
			ndl = dot(norm, L);
			lterm = std::max(ndl, 0.0f);
			color = brdf(vec3(), vec3(), trueIntersection.material->getColor());
			rr += color.x * color.x * lterm * (lightIntensity / d) *  dddd.x;
			gg += color.y * color.y * lterm * (lightIntensity / d) * dddd.y;
			bb += color.z * color.z * lterm * (lightIntensity / d) * dddd.z;
			oldNorm = norm;
			oldColor = color;
		}
		render.addSample(sample, vec3(rr, gg, bb));
		++counter;
		if (counter % 5000 == 0)
		{
			float progress = float(counter) / float(tnSamples);
			progress *= 100.0f;
			printf("We are  on ray number %u counter and %f percent done\n", counter.load(), progress);
		}
	}
}