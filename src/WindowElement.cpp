#include <WindowElement.h>

WindowElement::WindowElement(sf::Vector2f position, sf::Vector2f size, WindowElement *parent) : shape(size), parent(parent){
    shape.setPosition(position);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(3.0f);
}

void WindowElement::draw(sf::RenderWindow *window){
    window->draw(shape);
    for (auto child: children){
        child->draw(window);
    }
}

void WindowElement::receive_event(VrEventPublisher *publisher, std::unique_ptr<vr::Event> event){

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
