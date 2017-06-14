#include <WindowElementContainer.h>

WindowElementContainer::WindowElementContainer(){

};

WindowElementContainer::~WindowElementContainer(){

};

bool WindowElementContainer::init(std::string xml_path){

    std::string source = readFile(xml_path);

    pugi::xml_parse_result result = doc.load_string(source.c_str());

    if (result) {
        std::cout << "XML parsed without errors" << std::endl;
    }
    else {
        std::cout << "XML [" << source << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
        std::cout << "Error description: " << result.description() << "\n";
        std::cout << "Error offset: " << result.offset << " (error at [..." << (source.c_str() + result.offset) << "]\n\n";
        return false;
    }

    traverse(doc);
}

bool WindowElementContainer::begin(pugi::xml_node& node){
    std::cout << "START" << std::endl;
    return true;
}
bool WindowElementContainer::end(pugi::xml_node& node){
    std::cout << "END" << std::endl;
    return true;
}

bool WindowElementContainer::for_each(pugi::xml_node& node){

    for (int i = 0; i < depth(); ++i) std::cout << "  "; // indentation

    std::cout << "name='" << node.name() << "', value='" << node.value() << "' ,ATTRIBS: ";
    for (auto i = node.attributes_begin(); i != node.attributes_end(); i++){
        std::cout << i->value() << " | ";
    }

    std::cout << std::endl;

    return true; // continue traversal
}

void WindowElementContainer::traverse(pugi::xml_document& document){
    document.traverse(*this);
}

