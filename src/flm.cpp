#include "include/flm.hpp"
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

FLM::FLM()
    : open_flag(false),
      selected(0),
      scroll_offset(0),
      width_value(30),
      current_path(fs::current_path().string()) {}

bool FLM::is_open() const {
    return open_flag;
}

int FLM::width() const {
    return open_flag ? width_value : 0;
}

void FLM::toggle() {
    open_flag = !open_flag;
    if (open_flag)
        load_directory();
}

std::string FLM::truncate_path(const std::string& path, int width) {
    if ((int)path.size() <= width)
        return path;

    return "..." + path.substr(path.size() - (width - 3));
}

void FLM::load_directory() {
    entries.clear();

    try {
        if (fs::path(current_path).has_parent_path()) {
            entries.push_back({"..", true});
        }

        for (const auto& entry : fs::directory_iterator(current_path)) {
            entries.push_back({
                entry.path().filename().string(),
                entry.is_directory()
            });
        }

        std::sort(entries.begin(), entries.end(),
            [](const Entry& a, const Entry& b) {
                if (a.is_dir != b.is_dir)
                    return a.is_dir > b.is_dir;
                return a.name < b.name;
            });

        selected = 0;
        scroll_offset = 0;
    }
    catch (...) {
        entries.clear();
        entries.push_back({"[error to open the selected directory]", false});
        selected = 0;
    }
}

bool FLM::handle_input(int ch, std::string& selected_file) {
    if (!open_flag || entries.empty())
        return false;

    switch (ch) {
        case KEY_UP:
            if (selected > 0)
                selected--;
            break;

        case KEY_DOWN:
            if (selected < (int)entries.size() - 1)
                selected++;
            break;

        case 10:
        {
            auto& entry = entries[selected];

            if (entry.is_dir) {
                if (entry.name == "..")
                    current_path = fs::path(current_path).parent_path().string();
                else
                    current_path = (fs::path(current_path) / entry.name).string();

                load_directory();
            } else {
                selected_file = (fs::path(current_path) / entry.name).string();
                open_flag = false;
                return true;
            }
            break;
        }

        case KEY_BACKSPACE:
        case 'h':
            current_path = fs::path(current_path).parent_path().string();
            load_directory();
            break;
    }

    return false;
}

void FLM::render(int total_rows, int total_cols) {
    if (!open_flag)
        return;

    int width = width_value;

    attron(COLOR_PAIR(3));
    for (int y = 0; y < total_rows; y++) {
        mvhline(y, 0, ' ', width);
    }
    attroff(COLOR_PAIR(3));

    for (int y = 0; y < total_rows; y++) {
        mvaddch(y, width - 1, ACS_VLINE);
    }

    attron(A_BOLD);
    std::string path_display = truncate_path(current_path, width - 2);
    mvprintw(0, 1, "%s", path_display.c_str());
    attroff(A_BOLD);

    for (int x = 0; x < width - 1; x++)
        mvaddch(1, x, ACS_HLINE);

    int visible_rows = total_rows - 2;

    if (selected < scroll_offset)
        scroll_offset = selected;

    if (selected >= scroll_offset + visible_rows)
        scroll_offset = selected - visible_rows + 1;

    for (int i = 0; i < visible_rows; i++) {
        int entry_index = i + scroll_offset;
        if (entry_index >= (int)entries.size())
            break;

        int y = i + 2;

        if (entry_index == selected)
            attron(A_REVERSE);

        if (entries[entry_index].is_dir)
            mvprintw(y, 1, "> %s", entries[entry_index].name.c_str());
        else
            mvprintw(y, 3, "%s", entries[entry_index].name.c_str());

        if (entry_index == selected)
            attroff(A_REVERSE);
    }
}

