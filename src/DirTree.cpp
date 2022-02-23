#include "CairoLib.hpp"
#include <filesystem>
#include <sstream>  

namespace fs = std::filesystem;
using namespace CairoLib;

struct FileInfo {
    std::string path;
    bool isDir;
    long size;
};

bool isDir(FileInfo& a, FileInfo& b) {return a.isDir;}

void explore(std::string path, std::vector<FileInfo>& tree)
{
    tree.clear();
    for (const auto& entry : fs::directory_iterator(path)) 
    {
        std::stringstream fullpath(entry.path());
        std::string filename;
        while (std::getline(fullpath, filename, '/')) {}
        tree.push_back({
            filename,
            entry.is_directory(),
            (entry.is_regular_file()) ? (long)entry.file_size() : 0
        });
    }
    std::sort(tree.begin(), tree.end(), isDir);
    tree.insert(tree.begin(), {"↩", false, 0});
}

std::string getParentPath(std::string path)
{
    std::string parent{""};
    std::string fragment;
    std::stringstream current(path);
    std::vector<std::string> pieces;

    while (std::getline(current, fragment, '/'))
        pieces.push_back(fragment);

    for (size_t i{1}; i < pieces.size()-1; i++)
        parent += '/' + pieces[i];
    
    return parent;
}

setup_t DirTree = []()
{
    std::string path{"/home/me/samples"};
    std::vector<FileInfo> directory;
    explore(path, directory);
    
    return DrawControl(
        [directory, path](Component<Audio::MidiBuffer>* self) mutable
        {
            float font = self->label.size*.75;
            float box_height = font * (96.0f/72.0f);
            const Rect& base = self->rect;
            
            Rect textbox{ 
                base.x+spacing, 
                base.y+spacing,
                base.w-spacing*2, 
                box_height 
            };
            int maxLength = textbox.w / (font);

            DrawRect(base, fromHex(0x2f2f2f));
            DrawRect(textbox, fromHex(0xa3a3a3));
            
            Text field{ path, fromHex(0xffffff), (int)font, 4, (int)box_height+2 };
            CairoLib::DrawText( field, textbox, false, textbox.w-4);

            if ( (base.h) < (float)(directory.size()*box_height) ) {
                //needs a scrollbar;
            }
            int i{0};
            for (auto entry : directory)
            {
                Rect filebox{
                    textbox.x,
                    base.y+spacing*2+((i+1)*box_height),
                    textbox.w,
                    textbox.h
                };
                if ( (filebox.y+filebox.h) > base.h) break;
                
                bool active{CheckCollision(mouse_position, filebox)};

                DrawRect(filebox, (active) ? 
                    fromHex(0xddaa00) : (i%2 == 0) ? fromHex(0xa3a3a3) : fromHex(0xb0b0b0) 
                );

                Text field{ 
                    entry.path, 
                    (!entry.isDir || active) ? fromHex(0xffffff) : fromHex(0xaa5500),
                    (int)font, 4, (int)box_height+2 };
                CairoLib::DrawText( field, filebox, false, filebox.w-4);

                i++;
                if (active && GetMouseLeftClick())
                {
                    if (entry.isDir) {
                        path += '/'+entry.path;
                        explore(path, directory);
                    }
                    else if (entry.path == "↩") {
                        path = getParentPath(path);
                        explore(path, directory);
                    } 
                }
            }
        }
    );
};