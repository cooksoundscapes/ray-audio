#include "Library.h"
#include <filesystem>
#include <sstream>  

namespace fs = std::filesystem;

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
    tree.insert(tree.begin(), {"..", false, 0});
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
            float font = self->label.size;
            const Rectangle& base = self->rect;

            Rectangle textbox{ 
                base.x+spacing, 
                base.y+spacing,
                base.width-spacing*2, 
                font 
            };
            int maxLength = textbox.width / (font);

            DrawRectangleRec(base, fromHex(0x2f2f2f));
            DrawRectangleRec(textbox, fromHex(0xa3a3a3));
            DrawText( 
                TextSubtext(path.c_str(), 0, maxLength), 
                textbox.x+4 , textbox.y+2, font, WHITE 
            );
            if ( (base.height) < (float)(directory.size()*font) ) {
                //needs a scrollbar;
            }
            int i{0};
            for (auto entry : directory)
            {
                Rectangle filebox{
                    textbox.x,
                    base.y+spacing*2+((i+1)*font),
                    textbox.width,
                    textbox.height
                };
                if ( (filebox.y+filebox.height) > base.height) break;
                
                bool active{CheckCollisionPointRec(mouse_position, filebox)};

                DrawRectangleRec(filebox, (active) ? 
                    fromHex(0xddaa00) : (i%2 == 0) ? fromHex(0xa3a3a3) : fromHex(0xb0b0b0) 
                );
                DrawText(
                    TextSubtext(entry.path.c_str(), 0, maxLength), 
                    filebox.x*1.2f, filebox.y+2, font, WHITE 
                );
                i++;
                if (active && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    if (entry.isDir) {
                        path += '/'+entry.path;
                        explore(path, directory);
                    }
                    else if (entry.path == "..") {
                        path = getParentPath(path);
                        explore(path, directory);
                    } 
                }
            }
        }
    );
};