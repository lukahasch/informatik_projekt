#include <functional>
/* Bibliotheken einbinden */
#include <SPI.h>
#include <Wire.h>
/*
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
*/

/* Deklaration Pins */
#define CS 10   //Pin 10
#define RES 8   //Pin 8
#define RS 9    //Pin 9
#define SCK 13  //Pin 13
#define SDA 11  //Pin 11

/* Deklaration tft */
Adafruit_ST7735 tft = nullptr;

/* Deklaration Farben */
const uint16_t Black = 0x0000;
const uint16_t Blue = 0x001F;
const uint16_t Red = 0xF800;
const uint16_t Green = 0x07E0;
const uint16_t Cyan = 0x07FF;
const uint16_t Magenta = 0xF81F;
const uint16_t Yellow = 0xFFE0;
const uint16_t White = 0xFFFF;

void displayInit() {
  tft.initR(INITR_BLACKTAB); // Initialize ST7735 with black tab
  tft.fillScreen(Black);      // Fill the screen with black color
  tft.setRotation(1);         // Set rotation (0-3)
}

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

    Data() : nothing(0) {}
  } data;

  template<typename T> Message(T) = delete; // Prevent template instantiation

  Message(Type t) : type(t) {
    switch (type) {
    case Type::OpenMenu:
    case Type::CloseMenu:
    case Type::UnknownType:
      data.nothing = 0; // Initialize menu to a default value
      break;
    default:
      break;
    }
  }

  Message(Data::GameMessage gm) : type(Type::GameMessage) {
    data.gameMessage = gm;
  }

  static Message recv() {
    throw std::runtime_error("Message::recv() not implemented");
  }

  void send() {
    throw std::runtime_error("Message::send() not implemented");
  }

  static Message read(const std::function<int(int, int *)> &read) {
    int type_val;
    read(1, &type_val);
    Type msg_type = static_cast<Type>(type_val);

    switch (msg_type) {
    case Type::OpenMenu:
    case Type::CloseMenu: {
      int menu_val;
      read(1, &menu_val);
      Message msg(msg_type);
      msg.data.nothing = menu_val;
      return msg;
    }
    case Type::GameMessage: {
      int playerId;
      read(1, &playerId);
      // Read data_len first
      int data_len;
      read(1, &data_len);
      int *arr = new int[data_len];
      read(data_len, arr);
      Data::GameMessage gm;
      gm.playerId = playerId;
      gm.data = arr;
      gm.data_len = data_len;
      return Message(gm);
    }
    default:
      // Return a default message if type is unknown
      return Message(Type::UnknownType);
    }
  }

  void write(const std::function<void(int)> &write_function) {
    write_function(static_cast<int>(type));
    switch (type) {
    case Type::GameMessage:
      write_function(data.gameMessage.playerId);
      write_function(data.gameMessage.data_len);
      for (auto i = 0; i < data.gameMessage.data_len; ++i) {
        write_function(data.gameMessage.data[i]);
      }
      break;
    default:
      // For OpenMenu and CloseMenu, no additional data to write
      break;
    }
  }
};
