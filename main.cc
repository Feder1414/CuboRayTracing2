#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"   // Usado para el suelo.
#include "Cube.h"      // Implementación de la caja (cubo axis-aligned)
#include "RotateY.h" // Para rotar los cubos
#include <iostream>
#include <memory>

using std::make_shared;

int main()
{
    hittable_list world;

    // Suelo: se usa una esfera gigante para simular un plano infinito.
    auto ground_material = make_shared<lambertian>(color(0.3, 0.3, 0.3));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));


    for (int a = -5; a < 5; a++)
    {
        for (int b = -5; b < 5; b++)
        {
            auto   choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            // Para evitar solapamientos con un objeto específico
            if ((center - point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<material> cube_material;

                if (choose_mat < 0.8)
                {
                    // Difuso
                    auto albedo   = color::random() * color::random();
                    cube_material = make_shared<lambertian>(albedo);
                }
                else if (choose_mat < 0.95)
                {
                    // Metálico
                    auto albedo   = color::random(0.5, 1);
                    auto fuzz     = random_double(0, 0.5);
                    cube_material = make_shared<metal>(albedo, fuzz);
                }
                else
                {
                    // Vidrio
                    cube_material = make_shared<dielectric>(1.5);
                }
                point3 p0   = center - vec3(0.2, 0.2, 0.2);
                point3 p1   = center + vec3(0.2, 0.2, 0.2);
                auto   cube = make_shared<box>(p0, p1, cube_material);
                // Rotar el cubo 45° alrededor del eje Y.
                auto rotated_cube = make_shared<rotate_y>(cube, 45);
                world.add(rotated_cube);
            }
        }
    }

    double offsetX = 3.0;

    // Cubo metálico a la derecha al frente
    auto material_metal = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    {
        // Originalmente: de p0 = (4,0,1) a p1 = (6,2,3)
        // Aplicamos offsetX: p0 = (4 - offsetX, 0, 1), p1 = (6 - offsetX, 2, 3)
        point3 p0           = point3(4 - offsetX, 0, 1);
        point3 p1           = point3(6 - offsetX, 2, 3);
        auto   cube         = make_shared<box>(p0, p1, material_metal);
        auto   rotated_cube = make_shared<rotate_y>(cube, 45);
        world.add(rotated_cube);
    }

    // Cubo central vidrio centro de la escena
    auto material_glass = make_shared<dielectric>(1.5);
    {

        point3 p0           = point3(1 - offsetX, 0, -1);
        point3 p1           = point3(3 - offsetX, 2, 1);
        auto   cube         = make_shared<box>(p0, p1, material_glass);
        auto   rotated_cube = make_shared<rotate_y>(cube, 45);
        world.add(rotated_cube);
    }

    // Cubo cafe difuso fondo a la izquierda
    auto material_cafe = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    {

        point3 p0           = point3(-4 - offsetX, 0, -4);
        point3 p1           = point3(-2 - offsetX, 2, -2);
        auto   cube         = make_shared<box>(p0, p1, material_cafe);
        auto   rotated_cube = make_shared<rotate_y>(cube, 45);
        world.add(rotated_cube);
    }
    // Configuración de la cámara.
    camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 100; 
    cam.max_depth         = 100;
    cam.vfov              = 20;
    cam.lookfrom          = point3(13, 2, 3);
    cam.lookat            = point3(0, 0, 0);
    cam.vup               = vec3(0, 1, 0);
    cam.defocus_angle     = 0.6;
    cam.focus_dist        = 10.0;

    cam.render(world);
    return 0;
}
