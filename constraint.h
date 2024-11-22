#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "particle.h"
#include <cmath>

class Constraint {
public:
    Particle* p1;
    Particle* p2;
    float initial_length;
    bool active;

    Constraint(Particle* p1, Particle* p2) 
        : p1(p1), p2(p2), active(true) {
        initial_length = hypotf(p2->position.x - p1->position.x, p2->position.y - p1->position.y);
    }

    void satisfy() {
        if (!active) return;

        Vector2 delta = { p2->position.x - p1->position.x, p2->position.y - p1->position.y };
        float current_length = hypotf(delta.x, delta.y);
        float difference = (current_length - initial_length) / current_length;
        Vector2 correction = { delta.x * 0.5f * difference, delta.y * 0.5f * difference };

        if (!p1->is_pinned) {
            p1->position.x += correction.x;
            p1->position.y += correction.y;
        }
        if (!p2->is_pinned) {
            p2->position.x -= correction.x;
            p2->position.y -= correction.y;
        }
    }

    void deactivate() {
        active = false;
    }
};

#endif // CONSTRAINT_H
