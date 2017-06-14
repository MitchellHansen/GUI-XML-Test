#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include "util.hpp"
#include "Event.hpp"
#include "Input.h"
#include "Pub_Sub.h"
#include "pugixml.hpp"
#include <unordered_map>
#include <WindowElement.h>
#include <WindowElementContainer.h>


class Element {
public:
    Element(pugi::xml_node& reference) : node_reference(reference) {};
    ~Element(){};

    virtual void draw(sf::RenderWindow &window) = 0;


private:

    pugi::xml_node& node_reference;
    std::unordered_map<std::string, std::function<void()>> functor_handles;

};

int main() {

    WindowElementContainer states;
    states.init("../assets/index.giml");

    return 0;

    std::string html = readFile("../assets/index.giml");

    // Start taking tokens, pushing the full <TAG> to the stack
    // When coming upon a </TAG> token, pop the element on the stack

    WindowElement* root_element = new WindowElement(sf::Vector2f(0, 0), sf::Vector2f(600, 600), nullptr);
    root_element->tag = "HOME";
    root_element->data = "";

    WindowElement *current_element = root_element;

	std::mt19937 rng(time(NULL));
	std::uniform_int_distribution<int> rgen(100, 400);

	sf::RenderWindow window(sf::VideoMode(600, 600), "quick-sfml-template");
	window.setFramerateLimit(60);

	Input event_handler;

//	WindowElement *root_element = new WindowElement(sf::Vector2f(0, 0), sf::Vector2f(600, 600), nullptr);
//	root_element->children.push_back(new WindowElement(sf::Vector2f(10, 10), sf::Vector2f(100, 100), root_element));
//	root_element->children.push_back(new WindowElement(sf::Vector2f(200, 200), sf::Vector2f(150, 150), root_element));
//
//	root_element->subscribe_to_publisher(&event_handler, vr::Event::EventType::MouseButtonPressed);
//	for (int i = 0; i < root_element->children.size(); i++){
//		root_element->children.at(i)->subscribe_to_publisher(&event_handler, vr::Event::EventType::MouseButtonPressed);
//	}

	WindowHandler window_handler(&window);
	window_handler.subscribe_to_publisher(&event_handler, vr::Event::EventType::Closed);


	float physic_step = 0.166f;
	float physic_time = 0.0f;

	double frame_time = 0.0, elapsed_time = 0.0, delta_time = 0.0, accumulator_time = 0.0, current_time = 0.0;
	while (window.isOpen())
	{
		event_handler.consume_sf_events(&window);
		event_handler.handle_held_keys();
		event_handler.dispatch_events();

		elapsed_time = elap_time(); // Handle time
		delta_time = elapsed_time - current_time;
		current_time = elapsed_time;
		if (delta_time > 0.02f)
			delta_time = 0.02f;
		accumulator_time += delta_time;

		while (accumulator_time >= physic_step) { // While the frame has sim time, update

			accumulator_time -= physic_step;
			physic_time += physic_step;

			// Do physics at 60fps
		}

		window.clear(sf::Color::White);

		//root_element->draw(&window);

		window.display();



	}
	return 0;

}
