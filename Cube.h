#ifndef BOX_H
#define BOX_H
//==============================================================================================
// Implementación de una caja axis-aligned (que puede ser un cubo si sus lados son iguales)
// basada en el método de los "slabs".
// Inspirado en Ray Tracing in One Weekend de Peter Shirley.
//==============================================================================================

#include "hittable.h"
#include <algorithm> // Para std::swap
#include "rtweekend.h"

class box : public hittable
{
public:
    box() {}
    // p0: esquina mínima, p1: esquina máxima; mp es el material asignado a la caja.
    box(const point3& p0, const point3& p1, shared_ptr<material> mp) :
        box_min(p0), box_max(p1), mp(mp) {}

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const override
    {
        double tmin = ray_t.min;
        double tmax = ray_t.max;
        // Para cada eje (0 = x, 1 = y, 2 = z)
        for (int a = 0; a < 3; a++)
        {
            auto   invD = 1.0 / r.direction()[a];
            double t0   = (box_min[a] - r.origin()[a]) * invD;
            double t1   = (box_max[a] - r.origin()[a]) * invD;
            if (invD < 0.0)
                std::swap(t0, t1);
            tmin = t0 > tmin ? t0 : tmin;
            tmax = t1 < tmax ? t1 : tmax;
            if (tmax <= tmin)
                return false;
        }

        rec.t = tmin;
        rec.p = r.at(rec.t);

        // Determinar la normal de la cara impactada usando un bias.
        vec3         outward_normal;
        const double bias = 1e-4;
        if (std::fabs(rec.p.x() - box_min.x()) < bias)
            outward_normal = vec3(-1, 0, 0);
        else if (std::fabs(rec.p.x() - box_max.x()) < bias)
            outward_normal = vec3(1, 0, 0);
        else if (std::fabs(rec.p.y() - box_min.y()) < bias)
            outward_normal = vec3(0, -1, 0);
        else if (std::fabs(rec.p.y() - box_max.y()) < bias)
            outward_normal = vec3(0, 1, 0);
        else if (std::fabs(rec.p.z() - box_min.z()) < bias)
            outward_normal = vec3(0, 0, -1);
        else if (std::fabs(rec.p.z() - box_max.z()) < bias)
            outward_normal = vec3(0, 0, 1);
        else
            outward_normal = vec3(0, 0, 0); // Esto no debería ocurrir.

        rec.set_face_normal(r, outward_normal);
        rec.mat = mp;
        return true;
    }

    point3               box_min;
    point3               box_max;
    shared_ptr<material> mp;
};

#endif
