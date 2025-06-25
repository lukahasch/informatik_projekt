#pragma once
#include <cstdint>
#include <functional>

void setPixel(int x, int y, int color) {
  // This function sets a pixel at (x, y) to the specified color.
  // The implementation is not provided here, as it depends on the specific
  // display library used.
}

class PlayerClass {
public:
  int id = 0;
};



class Message {
public:
    enum class Type : int { OpenMenu, CloseMenu, GameMessage, UnknownType } type;

    union Data {
        int nothing;
        struct GameMessage {
            int playerId;
            int *data;
            int data_len;
        } gameMessage;

        Data();
    } data;

    Message(Type t);
    Message(Data::GameMessage gm);

    static Message recv();
    void send();
    static Message read(const std::function<int(int, int *)> &read);
    void write(const std::function<void(int)> &write_function);
};
