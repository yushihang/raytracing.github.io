
#include "ray.h"
#include "vec3.h"

#include <iostream>
#include <fstream>

#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
double hit_sphere(const point3 &center, double radius, const ray &r)
{
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
    {
        return -1.0; //没有相交
    }
    else
    {
        return (-half_b - sqrt(discriminant)) / a;
        //返回最近的交点信息
    }
}

color ray_color(const ray& r, const hittable& world) {
    
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1,1,1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main(int argv, char **argc)
{
    
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1000;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10000;
    
    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
 
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
    
    // Camera
    
    // Camera
    camera cam;
    
    std::fstream file;
    file.open("/Users/mac/Documents/GitHub/raytracing.github.io/Xcode/raytracing/common/test.ppm", std::ios::out);
    // Render
    
    file << "P3\n"
    << image_width << " " << image_height << "\n255\n";
    
    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            write_color(file, pixel_color, samples_per_pixel);
        }
    }
    
    file.close();
    
    std::cerr << "\nDone.\n";
}
