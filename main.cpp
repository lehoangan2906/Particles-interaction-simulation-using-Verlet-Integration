#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "particle.h"
#include "constraint.h"
#include "input_handler.h"

const int WIDTH = 1080;
const int HEIGHT = 720;
const float PARTICLE_RADIOUS = 10.0f;
const float GRAVITY = 10.0f;
const float TIME_STEP = 0.1f;
const int ROW = 15;
const int COL = 15;
const float REST_DISTANCE = 30.0f;

int main(){
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Verlet Integration");
	window.setFramerateLimit(60);

	std::vector<Particle> particles;
	std::vector<Constraint> constraints;

	// Create particles in a grid pattern
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			float x = j * REST_DISTANCE + WIDTH/3;
			float y = i * REST_DISTANCE + HEIGHT/3;
			bool pinned = (i == 0);
			particles.emplace_back(x, y, pinned);
		}
	}	

	for (int row = 0; row < ROW; row++) {
		for (int col = 0; col < COL; col++) {
			if (col < COL - 1) {
				// Horizontal constraint
				constraints.emplace_back(&particles[row * COL + col], &particles[row * COL + col + 1]);
			}

			if (row < ROW - 1) {
				// Vertical constraint
				constraints.emplace_back(&particles[row * COL + col], &particles[(row + 1) * COL + col]);
			}
		}
	}

	while (window.isOpen()){
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			// handle mouse click
			InputHandler::handle_mouse_click(event, particles, constraints);
		}

		// Apply gravity and update particles
		for (auto& particle : particles){
			particle.apply_force(sf::Vector2f(0, GRAVITY)); // Apply the gravity force on the particles
			particle.update(TIME_STEP);  // Verlet integration
			particle.constrain_to_bounds(WIDTH, HEIGHT); // constrain the objects to fall within boundaries
		}

		// iterates over a collection of constraints and calls the satisfy() method on each one
		// basically means ensuring that the particles are a fixed distance apart
		for (size_t i = 0; i < 5; i++) {
			for (auto& constraint : constraints) {
				constraint.satisfy();
			}
		}

		window.clear(sf::Color::Black);

		// Draw the particles as points
		for (const auto& particle: particles) {
			sf::Vertex point(particle.position, sf::Color::White);
			window.draw(&point, 1, sf::Points);
		}

		// Draw the constraints as lines
		for (const auto& constraint : constraints) {
			if (!constraint.active) continue;
			sf::Vertex line[] = {
				sf::Vertex(constraint.p1->position, sf::Color::White),
				sf::Vertex(constraint.p2->position, sf::Color::White),
			};
			window.draw(line, 2, sf::Lines);
		}

		window.display();
	}
}
