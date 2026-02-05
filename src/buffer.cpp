#include "include/buffer.hpp"

Buffer::Buffer() {
    lines.push_back("");
    modified = false;
}

void Buffer::insert_char(char c) {
    lines[cursor.y].insert(cursor.x, 1, c);
    cursor.x++;
    modified = true;
}

void Buffer::backspace() {
    if (cursor.x > 0) {
        lines[cursor.y].erase(cursor.x - 1, 1);
        cursor.x--;
        modified = true;
    } else if (cursor.y > 0) {
        cursor.x = lines[cursor.y - 1].size();
        lines[cursor.y - 1] += lines[cursor.y];
        lines.erase(lines.begin() + cursor.y);
        cursor.y--;
        modified = true;
    }
}

void Buffer::newline() {
    std::string rest = lines[cursor.y].substr(cursor.x);
    lines[cursor.y] = lines[cursor.y].substr(0, cursor.x);
    lines.insert(lines.begin() + cursor.y + 1, rest);
    cursor.y++;
    cursor.x = 0;
    modified = true;
}

int Buffer::line_count() const {
    return lines.size();
}

bool Buffer::is_modified() const {
    return modified;
}

void Buffer::set_modified(bool value) {
    modified = value;
}

