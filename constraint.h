#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "particle.h"
#include <cmath>
#include <limits>

// Constraint class to ensure that particles are a fixed distance apart
class Constraint {
	public:
		// Pointers to the two particles this constraint connects
		Particle *p1;
		Particle *p2;
		float initial_length; // original distance between particles
		bool active; // whether the constraint is active

		// Constructor initializes the constraint between two particles
		// Calculates and stores the initial distance between them
		Constraint(Particle *p1, Particle *p2) : p1(p1), p2(p2) { 
			initial_length = std::hypot(p2->position.x - p1->position.x, 
										p2->position.y - p1->position.y);
			active = true;
		}

		// Enforces the distance constraint between particles
		void satisfy() {
			if (!active) return;

			// Calculate the current vector between particles
			sf::Vector2f delta = p2->position - p1->position;
			
			// Calculate the current distance between particles
			float current_length = std::hypot(delta.x, delta.y);
			
			// Calculate how much the constraint is violated
			// (current_length - initial_length) gives the absolute error
			// Dividing by current_length normalizes the correction vector
			float difference = (current_length - initial_length) / current_length;
			
			// Calculate the correction vector
			// delta * difference gives the amount we need to move
			// * 0.5f splits the correction evenly between both particles
			sf::Vector2f correction = delta * difference * 0.5f;

			// Apply the correction to both particles in opposite directions
			// This maintains their center of mass while satisfying the constraint
			if (!p1->is_pinned) p1->position += correction;
			if (!p2->is_pinned) p2->position -= correction;
		}

		// Deactivate the constraint
		void deactivate() {
			active = false;
		}
};

#endif // CONSTRAINT_H
