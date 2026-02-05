#pragma once
#include <vector>
#include <string>

struct Cursor {
    int x = 0;
    int y = 0;
};

class Buffer {
public:
    std::vector<std::string> lines;
    Cursor cursor;

    Buffer();

    void insert_char(char c);
    void backspace();
    void newline();

    int line_count() const;
};
