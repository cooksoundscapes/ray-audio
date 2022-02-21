#pragma once
#include <string>
#include <SDL2/SDL.h>
#include "rapidxml.hpp"
#include "Library.hpp"

using namespace rapidxml;

class XmlParser
{
public:
    struct Container {
        std::string id;
        Flow isVertical;
        Rect rect;
        std::vector<float> division_table;
        int inverted;
    };
    using View = std::vector<BaseComponent*>;
    using BoxModel = std::vector<Container>;

    XmlParser() = default;
    XmlParser(std::string file, const Audio& cli, SDL_Renderer*);
    ~XmlParser();

    void drawNodeBoxes();
    void drawComponents();

    //traverses the DOM and call processNode() for each node;
    void render();
    std::string view_name;

private:
    std::vector<char> xml_copy;
    xml_document<> document;
    xml_node<>* root = nullptr;

    static long id_generator;
    View view;
    BoxModel boxModel;
    
    //for fetching the buffers;
    const Audio& audio_cli;

    //for access to the renderer
    SDL_Renderer* renderer;
    int rend_w, rend_h;
    int base_font_size;

    void clearView();
    void createId(xml_node<>*, std::string&);
    void processNode(xml_node<>*, int, int);
    
    void createComponent(xml_node<>*, int i);
    void createContainer(xml_node<>*, int i, int d);

    BaseComponent* getComponent(const std::string& id);
    const Container& getParentNode(xml_node<>*);
};