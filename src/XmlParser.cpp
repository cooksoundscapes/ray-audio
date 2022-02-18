#include "XmlParser.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <algorithm>

using NodeArray = std::vector<xml_node<>*>;

//overloading stream for Rectangle
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

NodeArray getChildren(xml_node<>*& parent)
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

Rectangle getElementBox(const XmlParser::Container& parent, int index)
{
    float ratio{ parent.division_table[index] };
    float stack_position{0};
    for (int i{0}; i < index; i++) {
        stack_position += parent.division_table[i];
    }
    Rectangle rect = {
        (parent.isVertical) ? parent.rect.x
         : (parent.rect.width * stack_position) + parent.rect.x,
        (!parent.isVertical) ? parent.rect.y
         : (parent.rect.height * stack_position) + parent.rect.y,
        (parent.isVertical) ? parent.rect.width
         : parent.rect.width * ratio,
        (!parent.isVertical) ? parent.rect.height
         : parent.rect.height * ratio
    };
    return rect;
};

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
    //calculate font based on window width:
    base_font_size = font_breakpoints.begin()->second;
    for (auto& bp : font_breakpoints) {
        if (window_width > bp.first)
            base_font_size = bp.second;
    }
    //std::cout << "At " << window_width << " width, font size is " << base_font_size << '\n';

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
        //count children and define division table;
        auto children = getChildren(node);
        std::vector<float> children_sizes;
        float accumulated{1};
        int dynamic_children{0};
        //comb children and define fixed values;
        for (auto& child : children) {
            auto isFixed = child->first_attribute("fixed");
            if (isFixed) {
                float fixed_size = std::stof(isFixed->value());
                children_sizes.push_back(fixed_size);
                accumulated -= fixed_size;
            } else {
                children_sizes.push_back(0);
                dynamic_children++;
            }
        }
        //search for zeros and divide the accumulated value by them;
        float dynamic_size{accumulated / dynamic_children};
        for (size_t i{0}; i < children_sizes.size(); i++) {
            if (children_sizes[i] == 0) {
                children_sizes[i] = dynamic_size;
            }
        }

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
            children_sizes,
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
        component->label.size = base_font_size;
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
            {c_label, RAYWHITE, base_font_size, 0, 0}
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
            {c_label, RAYWHITE, base_font_size, 0, 0}
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