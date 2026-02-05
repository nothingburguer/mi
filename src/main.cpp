#include "include/editor.hpp"
#include <ncurses.h>

int main(int argc, char** argv) {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);

    start_color();
    use_default_colors();

    init_pair(1, COLOR_BLACK, COLOR_BLUE);   // NORMAL
    init_pair(2, COLOR_BLACK, COLOR_GREEN);  // INSERT

    std::string file = (argc > 1) ? argv[1] : "";

    Editor editor(file);
    editor.run();

    endwin();
    return 0;
}

