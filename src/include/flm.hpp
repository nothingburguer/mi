#pragma once
#include <vector>
#include <string>
#include <ncurses.h>

class FLM {
public:
    FLM();

    void toggle();
    void render(int total_rows, int total_cols);
    bool handle_input(int ch, std::string& selected_file);

    bool is_open() const;
    int width() const;

private:
    struct Entry {
        std::string name;
        bool is_dir;
    };

    bool open_flag;
    int selected;
    int scroll_offset;
    int width_value;

    std::string current_path;
    std::vector<Entry> entries;

    void load_directory();
    std::string truncate_path(const std::string& path, int width);
};
