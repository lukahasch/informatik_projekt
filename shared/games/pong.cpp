#include <cstdint>
#include "../shared.hpp"

class PongMessage {
    public:
        enum class Type : int { PongUp, PongDown, BallMove, Point } type;
        union Data {
            struct PongUp {
                int playerId;
            } pongUp;

            struct PongDown {
                int playerId;
            } pongDown;

            struct BallMove {
                int x, y;
            } ballMove;

            struct Point {
                int playerId;
                int score;
            } point;

            Data() {}
        } data;

        static PongMessage pongUp(int playerId) {
            PongMessage msg;
            msg.type = Type::PongUp;
            msg.data.pongUp.playerId = playerId;
            return msg;
        }

        static PongMessage pongDown(int playerId) {
            PongMessage msg;
            msg.type = Type::PongDown;
            msg.data.pongDown.playerId = playerId;
            return msg;
        }

        static PongMessage ballMove(int x, int y) {
            PongMessage msg;
            msg.type = Type::BallMove;
            msg.data.ballMove.x = x;
            msg.data.ballMove.y = y;
            return msg;
        }

        static PongMessage point(int playerId, int score) {
            PongMessage msg;
            msg.type = Type::Point;
            msg.data.point.playerId = playerId;
            msg.data.point.score = score;
            return msg;
        }
        
        static PongMessage read(const std::function<int(int, int *)> &read) {
            int type_val;
            read(1, &type_val);
            Type msg_type = static_cast<Type>(type_val);

            PongMessage msg;
            msg.type = msg_type;

            switch (msg_type) {
            case Type::PongUp: {
                int playerId;
                read(1, &playerId);
                msg.data.pongUp.playerId = playerId;
                break;
            }
            case Type::PongDown: {
                int playerId;
                read(1, &playerId);
                msg.data.pongDown.playerId = playerId;
                break;
            }
            case Type::BallMove: {
                int x, y;
                read(1, &x);
                read(1, &y);
                msg.data.ballMove.x = x;
                msg.data.ballMove.y = y;
                break;
            }
            case Type::Point: {
                int playerId, score;
                read(1, &playerId);
                read(1, &score);
                msg.data.point.playerId = playerId;
                msg.data.point.score = score;
                break;
            }
            default:
                // Unknown type, leave as default
                break;
            }
            return msg;
        }

        void write(const std::function<void(int)> &write_function) const {
            write_function(static_cast<int>(type));
            switch (type) {
            case Type::PongUp:
                write_function(data.pongUp.playerId);
                break;
            case Type::PongDown:
                write_function(data.pongDown.playerId);
                break;
            case Type::BallMove:
                write_function(data.ballMove.x);
                write_function(data.ballMove.y);
                break;
            case Type::Point:
                write_function(data.point.playerId);
                write_function(data.point.score);
                break;
            default:
                // Unknown type, do nothing
                break;
            }
        }
};

void mega(std::function<Message::Data::GameMessage()> &read) {
    while (true) {
        Message::Data::GameMessage msg = read();

        PongMessage pongMsg = PongMessage::pongDown(msg.playerId);

        Message(pongMsg).send();
    }
}

void nano() {}