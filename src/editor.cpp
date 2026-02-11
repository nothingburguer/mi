#include "include/editor.hpp"
#include "include/buffer.hpp"
#include "include/file.hpp"
#include <ncurses.h>

Editor::Editor(const std::string& file)
    : mode(Mode::NORMAL), filename(file), row_offset(0) {
    if (!file.empty()) {
        File::load(file, buffer);
        buffer.set_modified(false);
    } else {
        buffer.lines.clear();
        buffer.lines.push_back("");

        buffer.cursor.x = 0;
        buffer.cursor.y = 0;

        buffer.set_modified(false);
        filename = "nullfile";
    }
}

void Editor::render_command_line() {
    attron(A_BOLD);

    mvhline(rows - 1, 0, ' ', cols);

    mvprintw(rows - 1, 0, ":%s", command.c_str());

    move(rows - 1, command.size() + 1);

    attroff(A_BOLD);
}

void Editor::render_status_bar() {
    int status_color = (mode == Mode::INSERT) ? 2 : 1;

    attron(COLOR_PAIR(status_color));
    attron(A_BOLD);

    mvhline(rows - 1, 0, ' ', cols);

    std::string mode_str =
        (mode == Mode::NORMAL) ? "NORMAL" : "INSERT";

    std::string left =
        "-- " + mode_str +
        " --  Ln " + std::to_string(buffer.cursor.y + 1) +
        ", Col " + std::to_string(buffer.cursor.x + 1);

    std::string right =
        filename.empty() ? "[No File]" : filename;

    if (buffer.is_modified()) {
        right += " [+]";
    }

    mvprintw(rows - 1, 0, "%s", left.c_str());

    int right_x = cols - right.size() - 1;
    if (right_x > (int)left.size())
        mvprintw(rows - 1, right_x, "%s", right.c_str());

    attroff(A_BOLD);
    attroff(COLOR_PAIR(status_color));
}

void Editor::render() {
    getmaxyx(stdscr, rows, cols);
    erase();

    int sidebar = flm.width();
    int editor_start_x = sidebar;

    // Renderiza buffer
    for (int i = 0; i < rows - 1; i++) {
        int line_idx = i + row_offset;

        if (line_idx < buffer.line_count()) {

            mvprintw(i, editor_start_x, "%4d ", line_idx + 1);

            mvprintw(i, editor_start_x + 5, "%s",
                     buffer.lines[line_idx].c_str());

        } else {
            attron(COLOR_PAIR(10));
            attron(A_DIM);

            mvprintw(i, editor_start_x, "   ~");

            attroff(COLOR_PAIR(10));
            attroff(A_DIM);
        }
    }

    if (mode == Mode::COMMAND) {
        render_command_line();
    } else {
        render_status_bar();
    }

    flm.render(rows, cols);

    int screen_y = buffer.cursor.y - row_offset;
    int screen_x = buffer.cursor.x + editor_start_x + 5;

    if (screen_y >= 0 && screen_y < rows - 1 &&
        screen_x >= editor_start_x) {
        move(screen_y, screen_x);
    }

    refresh();
}

void Editor::handleInput() {
    int ch = getch();

    if (mode == Mode::NORMAL && ch == 6) { // Ctrl+F
        flm.toggle();
        return;
    }

    if (flm.is_open() && mode != Mode::INSERT) {
        std::string file;
        if (flm.handle_input(ch, file)) {
            File::load(file, buffer);
            filename = file;
            buffer.set_modified(false);
        }
        return;
    }

    if (mode == Mode::INSERT) {
        switch (ch) {
            case 27:
                mode = Mode::NORMAL;
                break;

            case KEY_LEFT:
                if (buffer.cursor.x > 0)
                    buffer.cursor.x--;
                break;

            case KEY_RIGHT:
                if (buffer.cursor.x < (int)buffer.lines[buffer.cursor.y].size())
                    buffer.cursor.x++;
                break;

            case KEY_UP:
                if (buffer.cursor.y > 0) {
                    buffer.cursor.y--;
                    buffer.cursor.x = std::min(
                        buffer.cursor.x,
                        (int)buffer.lines[buffer.cursor.y].size()
                    );
                }
                break;

            case KEY_DOWN:
                if (buffer.cursor.y < buffer.line_count() - 1) {
                    buffer.cursor.y++;
                    buffer.cursor.x = std::min(
                        buffer.cursor.x,
                        (int)buffer.lines[buffer.cursor.y].size()
                    );
                }
                break;

            case KEY_BACKSPACE:
            case 127:
                buffer.backspace();
                break;

            case '\n':
                buffer.newline();
                break;

            case '\t':
                for (int i = 0; i < TAB_WIDTH; i++) {
                    buffer.insert_char(' ');
                }
                break;

            default:
                if (isprint(ch))
                    buffer.insert_char(ch);
        }
    }
    else if (mode == Mode::NORMAL) {
        switch (ch) {
            case 'i':
                mode = Mode::INSERT;
                break;

            case KEY_LEFT:
                if (buffer.cursor.x > 0)
                    buffer.cursor.x--;
                break;

            case KEY_RIGHT:
                if (buffer.cursor.x < (int)buffer.lines[buffer.cursor.y].size())
                    buffer.cursor.x++;
                break;

            case KEY_UP:
                if (buffer.cursor.y > 0) {
                    buffer.cursor.y--;
                    buffer.cursor.x = std::min(
                        buffer.cursor.x,
                        (int)buffer.lines[buffer.cursor.y].size()
                    );
                }
                break;

            case KEY_DOWN:
                if (buffer.cursor.y < buffer.line_count() - 1) {
                    buffer.cursor.y++;
                    buffer.cursor.x = std::min(
                        buffer.cursor.x,
                        (int)buffer.lines[buffer.cursor.y].size()
                    );
                }
                break;

            case ':':
                mode = Mode::COMMAND;
                command.clear();
                break;
        }
    }
    else if (mode == Mode::COMMAND) {
        if (ch == 27) { // ESC
            mode = Mode::NORMAL;
            command.clear();
        }
        else if (ch == KEY_BACKSPACE || ch == 127) {
            if (!command.empty())
                command.pop_back();
        }
        else if (ch == '\n') {
            if (command == "q") {
                endwin();
                exit(0);
            }

            else if (command == "flm") {
                flm.toggle();
                command.clear();
                mode = Mode::NORMAL;
                return;
            }

            else if (command.rfind("wq", 0) == 0) {
                std::string new_filename = filename;

                if (command.size() > 2 && command[2] == ' ')
                    new_filename = command.substr(3);

                if (!new_filename.empty()) {
                    if (File::save(new_filename, buffer)) {
                        filename = new_filename;
                        buffer.set_modified(false);
                        endwin();
                        exit(0);
                    }
                }
            }

            else if (command.rfind("w", 0) == 0) {
                std::string new_filename = filename;

                if (command.size() > 1 && command[1] == ' ')
                    new_filename = command.substr(2);

                if (!new_filename.empty()) {
                    if (File::save(new_filename, buffer)) {
                        filename = new_filename;
                        buffer.set_modified(false);
                    }
                }
            }

            command.clear();
            mode = Mode::NORMAL;
        }
        else if (isprint(ch)) {
            command.push_back(ch);
        }
    }
}

void Editor::run() {
    while (true) {
        render();
        handleInput();
        int rows, cols;
        getmaxyx(stdscr, rows, cols);

        if (buffer.cursor.y < row_offset)
            row_offset = buffer.cursor.y;

        if (buffer.cursor.y >= row_offset + rows - 1)
            row_offset = buffer.cursor.y - rows + 2;
    }
}

