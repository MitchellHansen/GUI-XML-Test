#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include "util.hpp"
#include "Event.hpp"
#include "Input.h"
#include "Pub_Sub.h"
#include "pugixml.hpp"


// There are two main ways that I can see to solve Z fighting on clicks and rendering.
// The first being the naive way of sending click events for every window element
// matching the clicks coordinates, and then checking for the highest Z element
// from those window elements.
// The second way could be using the tree structure to somehow check for Z level
// implicitly. Though this will remove the users ability to set Z level themselves....

// In order to do a Z check among all of the click subscribers we need to either,
// redo the way events are handled, or create some sort of central static class which
// will somehow store state of X elements for each frame

// vr::MouseButtonClickHighestZ
//      - When the user clicks a button, all of the intersecting panels are compared and
//        the one with the lowest Z value selected
//      - Currently all clicks are sent to every panel that looks for clicks. The panel then compares
//        itself to the click, or not. And act accordingly.
//      - This allows the panels to be really flexible, but now whenever we need info about the surrounding panels
//        such as Z height we have no way of getting it. So we will need some sort of shared asset that will
//        take all the z heights, and then send a vr::MouseButtonClickHighestZ event to the lowest Z subscriber
//      - Maybe access the shared asset through the pointer back to the event publisher. No everything is hidden
//        by the polymorphism

// vr::MouseMovedHighestZ


// I also need to handle clicking and dragging which probably is best handled by
// mouse held events and dynamic subscribing and unsubscribing to those events.


// Need to handle keyboard input and what constitutes the divide between focused
// and unfocused elements

struct giml_element {

    giml_element *parent;
    std::vector<giml_element> children;
    std::string tag;
    std::string data;
};

std::string read_file(std::string filename){

    std::ifstream file(filename);

    if (!file.is_open()){
        std::cout << "couldn't open file" << std::endl;
        abort();
    }

    std::string container;
    std::string line;

    while(std::getline(file, line)){
        container += line;
    }
    file.close();

    return container;
}

void print_nodes(const giml_element &root){
    std::cout << root.tag << std::endl;
    for (int i = 0; i < root.children.size(); i++){
        print_nodes(root.children.at(i));
    }
    std::cout << "/" << root.tag << std::endl;
}

class WindowElement : public VrEventSubscriber {
public:
	WindowElement(sf::Vector2f position, sf::Vector2f size, WindowElement* parent) : shape(size), parent(parent) {
		shape.setPosition(position);
		shape.setOutlineColor(sf::Color::Black);
		shape.setOutlineThickness(3.0f);
	}

	void draw(sf::RenderWindow *window){
		window->draw(shape);
		for (auto child: children){
			child->draw(window);
		}
	}

	void recieve_event(VrEventPublisher* publisher, std::unique_ptr<vr::Event> event){

		if (event->type == vr::Event::EventType::MouseButtonPressed){

			vr::MouseButtonPressed* mouse_data = static_cast<vr::MouseButtonPressed*>(event.get());

			sf::Vector2f pos = shape.getPosition();
			sf::Vector2f size = shape.getSize();

			if (mouse_data->x > pos.x && mouse_data->x < pos.x + size.x &&
				mouse_data->y > pos.y && mouse_data->y < pos.y + size.y ){

	            std::cout << "CLICK" << std::endl;

			}

		}

	}

	sf::RectangleShape shape;
	WindowElement *parent;
	std::vector<WindowElement*> children;
    std::string tag;
    std::string data;

};

class ElementState : pugi::xml_tree_walker{




    virtual bool for_each(pugi::xml_node& node){


    }


};

struct simple_walker: pugi::xml_tree_walker
{
    virtual bool for_each(pugi::xml_node& node)
    {
        for (int i = 0; i < depth(); ++i) std::cout << "  "; // indentation

        std::cout << "name='" << node.name() << "', value='" << node.value() << "' ,ATTRIBS: ";
        for (auto i = node.attributes_begin(); i != node.attributes_end(); i++){
            std::cout << i->value() << " | ";
        }

        std::cout << std::endl;

        return true; // continue traversal
    }
};

int main() {

    std::string source = read_file("../assets/index.giml");

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(source.c_str());

    if (result) {
        std::cout << "XML [" << source << "] parsed without errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n\n";
    }
    else {
        std::cout << "XML [" << source << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
        std::cout << "Error description: " << result.description() << "\n";
        std::cout << "Error offset: " << result.offset << " (error at [..." << (source.c_str() + result.offset) << "]\n\n";
    }

//    for (auto i : doc.children()){
//        std::cout << i.name() << std::endl;
//        std::cout << i.attribute("thing:int").value() << std::endl;
//    }

    simple_walker walker;
    doc.traverse(walker);

    std::string html = read_file("../assets/index.giml");

    return 0;
    // Start taking tokens, pushing the full <TAG> to the stack
    // When coming upon a </TAG> token, pop the element on the stack

    WindowElement* root_element = new WindowElement(sf::Vector2f(0, 0), sf::Vector2f(600, 600), nullptr);
    root_element->tag = "HOME";
    root_element->data = "";

    WindowElement *current_element = root_element;

    unsigned int position = 0;
    while (position < html.length()-1){

        size_t start_tag = html.find("<", position);
        size_t end_tag = html.find(">", position);

        if (html[start_tag+1] == '/'){

            current_element = current_element->parent;

        }
        else if (html[end_tag-1] == '/'){
            // Push the tag to the stack
            std::string tag = html.substr(start_tag, end_tag-start_tag+1);

            WindowElement* e = new WindowElement(sf::Vector2f(0, 0), sf::Vector2f(600, 600), nullptr);
            e->tag = tag;
            e->parent = current_element;

            current_element->children.push_back(e);
            current_element = current_element->children.back();
        } else {

            // Push the tag to the stack
            std::string tag = html.substr(start_tag, end_tag-start_tag+1);
			std::vector<std::string> tokens = split(tag, ' ');

			WindowElement* e = new WindowElement(sf::Vector2f(0, 0), sf::Vector2f(600, 600), nullptr);
            e->tag = tag;
            e->parent = current_element;

            current_element->children.push_back(e);
            current_element = current_element->children.back();

        }

        position = end_tag+1;
    }


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
