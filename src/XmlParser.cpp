#include "XmlParser.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <algorithm>

using NodeArray = std::vector<xml_node<>*>;

//overloading stream
std::ostream& operator <<(std::ostream& out, const Rectangle& r)
{
    out << r.x << ' ' << r.y << ' ' << r.width << ' ' << r.height;
    return out;
}

NodeArray filterNode(xml_node<>*& parent, std::string name)
{
    NodeArray output;
    for(
        auto* component = parent->first_node(name.c_str()); 
        component != nullptr; 
        component = component->next_sibling(name.c_str())
        )
    {
        output.push_back(component);
    } 
    return output;
}

//overload w/out filter
NodeArray filterNode(xml_node<>*& parent)
{
    NodeArray output;
    for(
        auto* component = parent->first_node(); 
        component != nullptr; 
        component = component->next_sibling()
        )
    {
        output.push_back(component);
    } 
    return output;
}

long XmlParser::id_generator{0};

XmlParser::XmlParser(std::string file, const Audio& cli) : audio_cli{cli} 
{
    std::ifstream stream{file};
    if (!stream.is_open()) {
        std::cout << "failed to open " << file << '\n';
        return;
    }
    std::copy(
        std::istreambuf_iterator<char>(stream), 
        std::istreambuf_iterator<char>(),
        std::back_inserter(xml_copy)
    );
    xml_copy.push_back('\0');

    document.parse<0>(&xml_copy[0]);

    root = document.first_node();

    std::cout << "Opening " << root->name() << '\n';
}

void XmlParser::clearView()
{
    for (auto& comp : view)
        delete comp;
    view.clear();
}

XmlParser::~XmlParser()
{
    clearView();
}

void XmlParser::render()
{
    int depth{0}, index{0};
    std::vector<int> index_pool;
    index_pool.reserve(256);
    boxModel.clear();
    xml_node<>* target = root->first_node();

    //start traversing the DOM;
    while (target)
    {
        processNode(target, depth, index);
        if (target->first_node()) {
            target = target->first_node();
            depth += 1;
            index_pool.push_back(index);
            index = 0;
        } else if (target->next_sibling()) {
            target = target->next_sibling();
            index += 1;
        } else {
            target = target->parent();
            depth -= 1;
            index = index_pool.back();
            index_pool.pop_back();
            while (!target->next_sibling() && depth > 0)
            {
                target = target->parent();
                depth -= 1;
                index = index_pool.back();
                index_pool.pop_back();
            }
            target = target->next_sibling();
            index += 1;
        }
    } 
}

const XmlParser::Container& XmlParser::getParentNode(xml_node<>* node)
{
    auto parent = node->parent();
    auto parent_id = parent->first_attribute("id");
    auto parentData = std::find_if(boxModel.begin(), boxModel.end(),
        [&parent_id](const auto& obj){
            return obj.id == parent_id->value();
        }
    );
    return *parentData;
}

BaseComponent* XmlParser::getComponent(const std::string& id)
{
    auto comp = std::find_if(view.begin(), view.end(),
        [&id](const auto& obj) {
            return obj->getId() == id;
        }
    );
    return *comp;
}

Rectangle getElementBox(const XmlParser::Container& parent, int index)
{
    Rectangle rect = {
        (parent.direction) ? parent.rect.x
         : (parent.rect.width  / parent.child_count) * index + parent.rect.x,
        (!parent.direction) ? parent.rect.y
         : (parent.rect.height / parent.child_count) * index + parent.rect.y,
        (parent.direction) ? parent.rect.width
         : (parent.rect.width) / parent.child_count,
        (!parent.direction) ? parent.rect.height
         : (parent.rect.height / parent.child_count)
    };
    return rect;
};

void XmlParser::createId(xml_node<>* node, std::string& id)
{  
    id = std::to_string(id_generator++);
    char* in_mem_id{document.allocate_string(id.c_str())};
    xml_attribute<>* id_attr{document.allocate_attribute("id", in_mem_id)};
    node->append_attribute(id_attr);
}

void XmlParser::processNode(xml_node<>* node, int depth, int index)
{    
    //std::cout << node->name() << '\n';
    bool isContainer = false;
    for (const auto& c_type : container_types) {
        if (node->name() == c_type.first) {
            isContainer = true;
            break;
        }
    }
    if (isContainer) {      
        //count children;
        auto children = filterNode(node);

        //fetch or generate ID;
        std::string id;
        auto old_id = node->first_attribute("id");
        if (old_id) id = old_id->value();
        else createId(node, id);

        //calculate the rectangle
        Rectangle rect;
        if (depth == 0) rect = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
        else {
            auto parentData = getParentNode(node);
            rect = getElementBox(parentData, index);
        }
        //calculating flow direction;
        auto direction = container_types.find(node->name())->second;
        
        //registering Container data on class;  
        boxModel.push_back({
            id,
            direction,
            rect, 
            children.size(),
            0
        });

    } else {
        /*process as component*/
        createComponent(node, index);
    }
}

void XmlParser::createComponent(xml_node<>* node, int index)
{
    if (!node->first_attribute("type")) {
        std::cout << "Component doesn't have a type!\n";
        return;
    } 
    auto parentData = getParentNode(node);

    std::string id;
    auto old_id = node->first_attribute("id");
    if (old_id) {
        //update only what's required and return;
        auto component = getComponent(old_id->value());
        component->rect = getElementBox(parentData, index);
        //here, we should define breakpoints and refer to them;
        //component->label.size = ...;
        return;
    }
    createId(node, id);

    std::string c_class{ node->name() };
    std::string c_type { node->first_attribute("type")->value() };
    std::string c_label{ node->first_attribute("label") ? node->first_attribute("label")->value() : "" };
    int channel{0};    
    if (node->first_attribute("channel"))
        channel = std::stoi(node->first_attribute("channel")->value());

    if (c_class == "Audio")
    {
        auto drawfunc = audio_types.find(c_type)->second;
        if (!drawfunc) {
            std::cout << "Unregistered component type " << c_type << '\n';
            return;
        }
        view.push_back(new Component<Audio::AudioBuffer>(
            id,
            audio_cli.getBuffer(channel),
            drawfunc,
            getElementBox(parentData, index),
            {c_label, RAYWHITE, 20, 0, 0}
        ));
    } else if (c_class == "Control")
    {
        auto setupfunc = control_types.find(c_type)->second;
        if (!setupfunc) {
            std::cout << "Unregistered component type " << c_type << '\n';
            return;
        }
        view.push_back(new Component<Audio::MidiBuffer>(
            id,
            audio_cli.getMidiBuffer(),
            setupfunc(),
            channel,
            getElementBox(parentData, index),
            {c_label, RAYWHITE, 20, 0, 0}
        ));  
    } else {
        std::cout << "Unregistered component class " << node->name() << '\n';
        return;
    }
}

void XmlParser::drawComponents()
{
    for (auto component : view) {
        component->draw();
    }
}

void XmlParser::drawNodeBoxes()
{
    for (const auto& node : boxModel)
        DrawRectangleLines(
            node.rect.x,
            node.rect.y,
            node.rect.width,
            node.rect.height,
            RAYWHITE
        );
}