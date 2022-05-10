#include "Collider.h"

RayCollider::RayCollider(Vector3 start, Vector3 dir, float len)
{
    origin = start;
    direction = dir;
    length = len;
    type = cType::Ray;
}

RayCollider::~RayCollider()
{
}
bool RayCollider::Intersects(Collider* other, Vector3& Intersection)
{
    if (other->type == cType::Circle) {
        CircleCollider* circle = (CircleCollider*)other;
        // ray origin to sphere center 
        Vector3 center = { circle->center.x, circle->center.y, 0 };
        Vector3 L = Vector3Subtract(center, origin);

        // Project sphere center onto ray 
        float t = Vector3DotProduct(L, direction);

        // Get sqr distance from sphere center to ray 
        float sqrMagDist = Vector3DotProduct(L, L) - t * t;

        // Subtract penetration amount from projected distance 
        t -= sqrtf((circle->radius * circle->radius) - sqrMagDist);

        // If within ray length 
        if (t >= 0 && t <= length)
        {
            // Store intersection point
            Vector3 dir = Vector3(direction);
            dir.x *= t;
            dir.y *= t;
            dir.z *= t;
            Intersection = Vector3Add(origin, dir);
            return true;
        }

        // default no intersection 
        return false;
    }
    else if (other->type == cType::Rectangle) {
        RectangleCollider* rec = (RectangleCollider*)other;

        // get distances to each axis of the box 
        float xmin, xmax, ymin, ymax;

        // get min and max in the x-axis  
        if (direction.x < 0)
        {
            // (you may need to change the access modifiers of AABB.min/max) 
            xmin = (rec->max.x - origin.x) / direction.x;
            xmax = (rec->min.x - origin.x) / direction.x;
        }
        else
        {
            xmin = (rec->min.x - origin.x) / direction.x;
            xmax = (rec->max.x - origin.x) / direction.x;
        }

        // get min and max in the y-axis 
        if (direction.y < 0)
        {
            ymin = (rec->max.y - origin.y) / direction.y;
            ymax = (rec->min.y - origin.y) / direction.y;
        }
        else
        {
            ymin = (rec->min.y - origin.y) / direction.y;
            ymax = (rec->max.y - origin.y) / direction.y;
        }

        // ensure within box 
        if (xmin > ymax || ymin > xmax)
            return false;

        // the first contact is the largest of the two min 
        float t = fmaxf(xmin, ymin);
        t = abs(t);
        // intersects if within range 
        if (t >= 0 && t <= length)
        {
            // store intersection point if requested 
            Vector3 dir = Vector3(direction);
            dir.x *= t;
            dir.y *= t;
            dir.z *= t;
            Intersection = Vector3Add(origin, dir);

            DrawCircle(Intersection.x, Intersection.y, 10, RED);
            return true;
        }

        // not within Ray's range 
        return false;
    }
}



Vector2 RayCollider::ClosestPoint(Vector2 point){
    // Ray origin to point 
    Vector2 p = Vector2Subtract(point, { origin.x, origin.y });

    // Project the point onto the ray and clamp by length 
    float t = Clamp(Vector2DotProduct(p, { direction.x, direction.y }), 0, length);

    direction.x *= t;
    direction.y *= t;

    // Return position in direction of ray 
    return Vector2Add({ origin.x, origin.y }, { direction.x, direction.y });
}
