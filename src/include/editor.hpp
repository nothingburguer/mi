#pragma once
#include "buffer.hpp"
#include <string>

constexpr int TAB_WIDTH = 4;

enum class Mode {
    NORMAL,
    INSERT,
    COMMAND
};

class Editor {
public:
    Editor(const std::string& file);
    void run();

private:
    Buffer buffer;
    Mode mode;
    std::string filename;
    std::string command;
    std::string command_buffer;

    int row_offset;
    int rows;
    int cols;

    void render();
    void render_command_line();
    void render_status_bar();
    void handleInput();
};

