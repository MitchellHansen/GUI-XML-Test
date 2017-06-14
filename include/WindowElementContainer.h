#pragma once
#include <iostream>
#include <pugixml.hpp>
#include <vector>
#include <unordered_map>
#include "WindowElement.h"
#include "util.hpp"

// Window Element Container

// A container class which holds ownership to the XML tree. Each tree node will have a WindowElement
// class instantiated with a reference to that xml tree node. These WindowElements will be held


// Window Element pure virtual class whi

// Inheritable to classes which extend their rendering




class WindowElementContainer : private pugi::xml_tree_walker{

public:
    WindowElementContainer();
    ~WindowElementContainer();

    bool init(std::string xml_path);


    std::vector<WindowElement*>::iterator element_bucket_iterator(std::string element_name){



        return element_map.at(element_name).begin();
    };

private:

    virtual bool begin(pugi::xml_node& node);
    virtual bool end(pugi::xml_node& node);
    virtual bool for_each(pugi::xml_node& node);
    void traverse(pugi::xml_document& document);


    pugi::xml_document doc;



    // A map that puts elements in buckets according to their element tag
    std::unordered_map<std::string, std::vector<WindowElement*>> element_map;
    std::vector<std::unique_ptr<WindowElement>> element_list;


};



// Window elements are owned by the container class and destruct upon deletion from the element list.
// Other sections of code can have WindowElements, but when given to the container, it must be std::moved

// The problem here is that we now have state data in the container and will be passing it around to different
// sections of code with all sorts of different lifetimes. If it is removed from the element_list it would leave
// a dangling pointer, or if concurrency is in the future, it will cause problems with data races.

// The best way to combat this is to make it non-mutable or provide some sort of atomic interface for other
// classes to interact with the WindowElements contained therein. This comes with problems on it's own. Mainly
// maintenance, where all future WindowElement functionality will have to be wrapped by the Container.

// We could combat this shortfall by minimizing the interaction needed through the container, almost in a
// state machine kinda way. Then we could utilize the event system to preform all of the WindowElement specific
// event handlers

// Another problem. Creation of the WindowElements is handled at first by the XML parser, and then by the user.
// Deletion is handled by either the program, or the user. First problem with this is that when the user adds
// a WindowElement the XML tree will need to be updated in kind. This is doable, the library seems to provide tools,
// to make it relatively painless, but good to keep in mind. The other problem is slightly trickier, how the hell
// are we going to allow elements to be deleted and removed from the element list / XML tree.

// We could mirror the XML tree with our own tree of WindowElement*. That would allow us 
//







