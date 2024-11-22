#ifndef PARTICLE_H
#define PARTICLE_H

#include "raylib.h"

class Particle {
public:
    Vector2 position;
    Vector2 previous_position;
    Vector2 acceleration;
    bool is_pinned;

    Particle(float x, float y, bool pinned = false) 
        : position{ x, y }, previous_position{ x, y }, 
          acceleration{ 0, 0 }, is_pinned(pinned) {}

    void apply_force(const Vector2& force) {
        if (!is_pinned) {
            acceleration.x += force.x;
            acceleration.y += force.y;
        }
    }

    void update(float time_step) {
        if (!is_pinned) {
            Vector2 velocity = { position.x - previous_position.x, position.y - previous_position.y };
            previous_position = position;
            position.x += velocity.x + acceleration.x * time_step * time_step;
            position.y += velocity.y + acceleration.y * time_step * time_step;
            acceleration = { 0, 0 }; // Reset acceleration
        }
    }

    void constrain_to_bounds(float width, float height) {
        if (position.x < 0) position.x = 0;
        if (position.x > width) position.x = width;
        if (position.y < 0) position.y = 0;
        if (position.y > height) position.y = height;
    }
};

#endif // PARTICLE_H
