#ifndef BRIMEARSPOINT_HPP
#define BRIMEARSPOINT_HPP

#include <libslic3r/Point.hpp>


namespace Slic3r {

// An enum to keep track of where the current points on the ModelObject came from.
enum class PointsStatus {
    NoPoints,           // No points were generated so far.
    Generating,     // The autogeneration algorithm triggered, but not yet finished.
    AutoGenerated,  // Points were autogenerated (i.e. copied from the backend).
    UserModified    // User has done some edits.
};

struct BrimPoint
{
    Vec3f pos;
    float head_front_radius;
    int   volume_idx;

    BrimPoint()
        : pos(Vec3f::Zero()), head_front_radius(0.f), volume_idx(-1)
    {}

    BrimPoint(float pos_x,
                 float pos_y,
                 float pos_z,
                 float head_radius,
                 int volume_idx = -1)
        : pos(pos_x, pos_y, pos_z)
        , head_front_radius(head_radius)
        , volume_idx(volume_idx)
    {}

    BrimPoint(Vec3f position, float head_radius, int volume_idx = -1)
        : pos(position)
        , head_front_radius(head_radius)
        , volume_idx(volume_idx)
    {}

    Vec3f transform(const Transform3d &trsf)
    {
        Vec3d result = trsf * pos.cast<double>();
        return result.cast<float>();
    }

    void set_transform(const Transform3d& trsf)
    {
        pos = transform(trsf);
    }

    bool operator==(const BrimPoint &sp) const
    {
        float rdiff = std::abs(head_front_radius - sp.head_front_radius);
        return (pos == sp.pos) && rdiff < float(EPSILON);
    }

    bool operator!=(const BrimPoint &sp) const { return !(sp == (*this)); }
    template<class Archive> void serialize(Archive &ar)
    {
        ar(pos, head_front_radius, volume_idx);
    }
};

using BrimPoints = std::vector<BrimPoint>;

}


#endif // BRIMEARSPOINT_HPP