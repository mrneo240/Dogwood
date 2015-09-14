#pragma once

#include "Math/Algebra.h"

class Particle
{
public:
    void        SetMass(float mass);
    void        SetInverseMass(float inverseMass);
    
    // Integrates the particle forward in time (i.e. updates position and velocity)
    void        Integrate(float deltaTime);

protected:
    Vector3     m_position;
    Vector3     m_velocity;
    Vector3     m_acceleration;

    // Inverse mass is used here instead of simply mass, so that we can represent "infinite" mass (with an
    // inverse mass of 0), and *cannot* represent zero mass (which we don't want to allow).
    float       m_inverseMass;

    // Damping is required to remove energy added from numerical instability in physics integration step.
    const float DAMPING = 0.999f;
};