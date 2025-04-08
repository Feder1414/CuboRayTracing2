#pragma once
#ifndef ROTATE_Y_H
#    define ROTATE_Y_H

#    include "hittable.h"
#    include "rtweekend.h"
#    include <memory>

class rotate_y : public hittable
{
public:
    rotate_y(shared_ptr<hittable> p, double angle) :
        ptr(p)
    {
        auto radians = degrees_to_radians(angle);
        sin_theta    = sin(radians);
        cos_theta    = cos(radians);
        // Aquí podrías calcular la caja envolvente rotada, pero para simplicidad la omitimos.
    }

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const override
    {
        auto origin    = r.origin();
        auto direction = r.direction();
        // Rotamos el rayo en el espacio inverso para transformar al sistema local del objeto.
        origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
        origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

        direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
        direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

        ray rotated_r(origin, direction);

        if (!ptr->hit(rotated_r, ray_t, rec))
            return false;

        // Rotar el punto de impacto y la normal de vuelta al espacio global.
        auto p      = rec.p;
        auto normal = rec.normal;
        p[0]        = cos_theta * rec.p[0] + sin_theta * rec.p[2];
        p[2]        = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

        normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
        normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

        rec.p = p;
        rec.set_face_normal(rotated_r, normal);
        return true;
    }

private:
    shared_ptr<hittable> ptr;
    double               sin_theta;
    double               cos_theta;
};

#endif
