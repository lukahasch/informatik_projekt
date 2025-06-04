void setPixel(int x, int y, int color) {
    // This function sets a pixel at (x, y) to the specified color.
    // The implementation is not provided here, as it depends on the specific display library used.
}


class PlayerClass {
public:
    int id;
};

// Avoid using <variant> and instead use a simple enum and struct approach

class Message {
public:
    enum Type {
        OpenMenu,
        CloseMenu,
        GameMessage,
        UnknownType,
    } type;

    union Data {
        int nothing;
        struct GameMessage {
            int playerId;
            int* data;
            int data_len;
        }gameMessage;
    } data;

    Message(Type t) : type(t) {
        switch (type) {
            case OpenMenu:
            case CloseMenu:
            case UnknownType:
                data.nothing = 0; // Initialize menu to a default value
                break;
            default:
                break;
        }
    }

    Message(Data::GameMessage gm) : type(GameMessage) {
        data.gameMessage = gm;
    }

    static Message read(int (*read)(int, int*)) {
        // Read the message type
        int type_val;
        read(1, &type_val);
        Type msg_type = static_cast<Type>(type_val);

        switch (msg_type) {
            case OpenMenu:
            case CloseMenu: {
                int menu_val;
                read(1, &menu_val);
                Message msg(msg_type);
                msg.data.nothing = menu_val;
                return msg;
            }
            case GameMessage: {
                int playerId;
                read(1, &playerId);
                // Read data_len first
                int data_len;
                read(1, &data_len);
                int* arr = new int[data_len];
                read(data_len, arr);
                Data::GameMessage gm;
                gm.playerId = playerId;
                gm.data = arr;
                gm.data_len = data_len;
                return Message(gm);
            }
            default:
                // Return a default message if type is unknown
                return Message(UnknownType);
        }
    }

    void write(void (*write_function)(int)) {
        write_function(type);
        switch (type) {
            case GameMessage:
                write_function(data.gameMessage.playerId);
                write_function(data.gameMessage.data_len);
                for (int i = 0; i < data.gameMessage.data_len; ++i) {
                    write_function(data.gameMessage.data[i]);
                }
                break;
            default:
                // For OpenMenu and CloseMenu, no additional data to write
                break;
        }
    }
};

