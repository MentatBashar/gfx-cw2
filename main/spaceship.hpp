#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include <vector>

#include <cstdlib>

#include "mesh.hpp"

#include "../vmlib/vec3.hpp"
#include "../vmlib/mat33.hpp"
#include "../vmlib/mat44.hpp"

MeshData move_spaceship(MeshData spaceship_mesh, float t, Vec3f* pos);

MeshData make_spaceship();

MeshData make_cube(
    Vec3f aColor = { 1.f, 1.f, 1.f },
    Mat44f aPreTransform = kIdentity44f
    );

MeshData make_cylinder(
    bool aCapped = true,
    std::size_t aSubdivs = 16,
    Vec3f aColor = { 1.f, 1.f, 1.f },
    Mat44f aPreTransform = kIdentity44f
    );

MeshData make_cone(
    bool aCapped = true,
    std::size_t aSubdivs = 16,
    Vec3f aColor = { 1.f, 1.f, 1.f },
    Mat44f aPreTransform = kIdentity44f
    );

#endif // SPACESHIP_HPP
