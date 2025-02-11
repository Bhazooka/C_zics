#include "raylib.h"
#include <vector>
#include <iostream>

#include "particle.h"
#include "constraint.h"
#include "input_handler.h"
#include <raymath.h>

const int WIDTH = 940;
const int HEIGHT = 1040;
const float PARTICLE_RADIUS = 3.0f;
const float GRAVITY = 10.0f;
const float TIME_STEP = 0.1f;

const int ROW = 30;
const int COL = 30;
const float REST_DISTANCE = 15.0f;
const float GRAB_RADIUS = 5.0f;

Particle* grabbedParticle = nullptr;

int main() {
    InitWindow(WIDTH, HEIGHT, "Cloth Simulation");
    SetTargetFPS(60);

    std::vector<Particle> particles;
    std::vector<Constraint> constraints;

    //Initialize particles
    for (int row = 0; row < ROW; row++) {
        for (int col = 0; col < COL; col++) {
            float x = col * REST_DISTANCE + WIDTH / 3;
            float y = row * REST_DISTANCE + HEIGHT / 3;
            bool pinned = (row == 0);
            particles.emplace_back(x, y, pinned);
        }
    }

    //Initialize constraints
    for (int row = 0; row < ROW; row++) {
        for (int col = 0; col < COL; col++) {
            if (col < COL - 1) {
                //Horizontal constraint
                constraints.emplace_back(&particles[row * COL + col], &particles[row * COL + col + 1]);
            }
            if (row < ROW - 1) {
                // Vertical constraint
                constraints.emplace_back(&particles[row * COL + col], &particles[(row + 1) * COL + col]);
            }
        }
    }

    while (!WindowShouldClose()) {
        Vector2 mousePosition = GetMousePosition();

        //Tear cloth with right-click drag
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            InputHandler::tear_cloth(mousePosition.x, mousePosition.y, particles, constraints);
        }

        //Grab particle with left-click
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (auto& particle : particles) {
                float distance = Vector2Distance({ particle.position.x, particle.position.y }, mousePosition);
                if (distance <= GRAB_RADIUS) { // Use larger grab radius
                    grabbedParticle = &particle;
                    break;
                }
            }
        }

        //Drag grabbed particle
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && grabbedParticle) {
            grabbedParticle->position = { mousePosition.x, mousePosition.y };
        }

        //Release grabbed particle
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            grabbedParticle = nullptr;
        }

        //Apply gravity and update particles
        for (auto& particle : particles) {
            if (&particle != grabbedParticle) { //Skip grabbed particle for physics
                particle.apply_force({ 0, GRAVITY });
                particle.update(TIME_STEP);
                particle.constrain_to_bounds(WIDTH, HEIGHT);
            }
        }

        //Satisfy constraints
        for (size_t i = 0; i < 5; i++) {
            for (auto& constraint : constraints) {
                constraint.satisfy();
            }
        }

        //Render simulation
        BeginDrawing();
        ClearBackground(BLACK);

        //Draw particles
        for (const auto& particle : particles) {
            Color color = (&particle == grabbedParticle) ? RED : WHITE; // Highlight grabbed particle
            DrawCircle(static_cast<int>(particle.position.x), static_cast<int>(particle.position.y), PARTICLE_RADIUS, color);
        }

        //Draw constraints
        for (const auto& constraint : constraints) {
            if (!constraint.active) continue;
            DrawLine(static_cast<int>(constraint.p1->position.x), static_cast<int>(constraint.p1->position.y),
                    static_cast<int>(constraint.p2->position.x), static_cast<int>(constraint.p2->position.y), WHITE);
        }

        EndDrawing();
    }


    CloseWindow();
    return 0;
}





/*
TODO:
    - Toggle gravity On/Off  (PRESS G) -> objects in motion must continue with their direction when grave off
    - Mouse repelant, Press button to make the moouse a repelant to all the particles
    - Mouse attraction
    - Restart the simulation (PRESS R)
    - Add control labels
*/
