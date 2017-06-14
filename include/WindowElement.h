#pragma once
#include <SFML/Graphics.hpp>
#include <Pub_Sub.h>


class WindowElement : public VrEventSubscriber {
public:
    WindowElement(sf::Vector2f position, sf::Vector2f size, WindowElement* parent);
    void draw(sf::RenderWindow *window);
    void receive_event(VrEventPublisher *publisher, std::unique_ptr<vr::Event> event);

    sf::RectangleShape shape;
    WindowElement *parent;
    std::vector<WindowElement*> children;
    std::string tag;
    std::string data;

};
