#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>


#define CS 10
#define RES 8
#define RS 9
#define SCK 13
#define SDA 11


/* Taster PINS */
const char UP_BUTTON = A1;
const char DOWN_BUTTON = A3;
const char UP_BUTTON_TWO = A0;
const char DOWN_BUTTON_TWO = A2;


/* Variablen */
const unsigned int BALL_RATE = 50;
const unsigned int PADDLE_HEIGHT = 35;


unsigned char SCREEN_WIDTH = 128;
unsigned char SCREEN_HEIGHT = 160;


unsigned int playerOne = 0;
unsigned int playerTwo = 0;


/* Deklaration tft, verbindung zum I2C (SDA, SCL pins) */
Adafruit_ST7735 tft = Adafruit_ST7735(CS, RS, SDA, SCK, RES);



/* Position vom Ball, Paddle und Spieler auf dem tft */


unsigned char ballX = 64, ballY = 75;
unsigned char ballDir_x = 1, ballDir_y = 1;


const unsigned char PLAYER_TWO_X = 12;
unsigned char playerTwo_y = 65;


const unsigned char PLAYER_ONE_X = 115;
unsigned char playerOne_y = 65;


unsigned long ballUpdate;


const uint16_t Black = 0x0000;
const uint16_t Blue = 0x001F;
const uint16_t Red = 0xF800;
const uint16_t Green = 0x07E0;
const uint16_t Cyan = 0x07FF;
const uint16_t Magenta = 0xF81F;
const uint16_t Yellow = 0xFFE0;
const uint16_t White = 0xFFFF;


uint16_t Backround_Color = Black;


void setup() {
  SCREEN_WIDTH = SCREEN_WIDTH - 1;
  SCREEN_HEIGHT = SCREEN_HEIGHT - 1;


  Serial.begin(9600);
  tft.initR(INITR_BLACKTAB);
  // tft.setRotation(2);
  tft.fillScreen(Magenta);
  tft.fillScreen(Red);
  tft.fillScreen(Yellow);
  tft.fillScreen(Green);
  tft.fillScreen(Cyan);
  tft.fillScreen(Blue);
  tft.fillScreen(Backround_Color);
  unsigned long start = millis();
  delay(1000);
  // tft.fillScreen(Backround_Color);


  /* Pong Spiel Namen anzeigen */
  tft.setTextSize(2);
  tft.setTextColor(White);
  tft.setCursor(5, 70);
  tft.println("Pong Spiel");
  delay(1000);
  tft.fillScreen(Backround_Color);


  /* Ball Timer zuweisen*/
  ballUpdate = millis();
  delay(1000);


  /* Taster eins Initialisieren */
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);


  /* Taster zwei */
  pinMode(UP_BUTTON_TWO, INPUT_PULLUP);
  pinMode(DOWN_BUTTON_TWO, INPUT_PULLUP);
}


/* Punkteanzeige */
void drawScore() {
  tft.setTextSize(2);
  tft.setTextColor(Red);
  if (playerOne < 100)
    tft.setCursor(43, 0);
  else
    tft.setCursor(31, 0);
  tft.println(playerOne);
  tft.setTextColor(Blue);
  tft.setCursor(73, 0);
  tft.println(playerTwo);
}


/* Punkte zurücksetzten */
void eraseScore() {
  tft.setTextSize(2);
  tft.setTextColor(Black);
  if (playerOne < 100)
    tft.setCursor(43, 0);
  else
    tft.setCursor(31, 0);
  tft.println(playerOne);


  tft.setCursor(73, 0);
  tft.println(playerTwo);
}


void loop() {
  drawScore();


  bool update = false;            // Ein bool enthält einen von zwei Werten, true oder false
  unsigned long time = millis();  // Zeit-Variable


  static bool UP_STATE = false;
  static bool DOWN_STATE = false;
  static bool UP_STATE_TWO = false;
  static bool DOWN_STATE_TWO = false;


  UP_STATE |= (digitalRead(UP_BUTTON) == LOW);  // Taster auslesen
  DOWN_STATE |= (digitalRead(DOWN_BUTTON) == LOW);
  UP_STATE_TWO |= (digitalRead(UP_BUTTON_TWO) == LOW);
  DOWN_STATE_TWO |= (digitalRead(DOWN_BUTTON_TWO) == LOW);


  /* Neue Ballposition */
  if (time > ballUpdate) {
    unsigned char New_x = ballX + ballDir_x;  // (x+1)
    unsigned char New_y = ballY + ballDir_y;  // (y+1)


    /* Checkt ob die vertikalen Wände berührt werden */
    if (New_x == 0)  // Wand links
    {
      ballDir_x = -ballDir_x;  // Wechselt die Richtung
      New_x += ballDir_x + ballDir_x;
      eraseScore();  // Punkt für Spieler
      playerOne = playerOne + 1;
      drawScore();
      New_x = 64;  // Ball Reset
      New_y = 75;
    }


    if (New_x == SCREEN_WIDTH)  // Wand rechts
    {
      ballDir_x = -ballDir_x;  // Wechselt die Richtung
      New_x += ballDir_x + ballDir_x;
      eraseScore();  // Punkt für Spieler 2
      playerTwo = playerTwo + 1;
      drawScore();
      New_x = 64;  // Ball Reset
      New_y = 75;
    }


    // Checkt ob die horizontalen Wände berührt werden
    if (New_y == 0 || New_y == SCREEN_HEIGHT) {
      ballDir_y = -ballDir_y;  // Wechselt die Richtung
      New_y += ballDir_y + ballDir_y;
    }


    // Checkt, ob der Spieler 1 Paddel getroffen wurde
    if (New_x == PLAYER_ONE_X && New_y >= playerOne_y && New_y <= playerOne_y + PADDLE_HEIGHT) {
      ballDir_x = -ballDir_x;
      New_x += ballDir_x + ballDir_x;
    }


    // Checkt, ob der Spieler 2 Paddel getroffen wurde
    if (New_x == PLAYER_TWO_X && New_y >= playerTwo_y && New_y <= playerTwo_y + PADDLE_HEIGHT) {
      ballDir_x = -ballDir_x;
      New_x += ballDir_x + ballDir_x;
    }
    // tft.fillCircle(ballX, ballY, 2, Black);
    // tft.fillCircle(New_x, New_y, 2, White);
    tft.drawPixel(ballX, ballY, Black);
    tft.drawPixel(New_x, New_y, White);
    ballX = New_x;
    ballY = New_y;


    ballUpdate += BALL_RATE;
    update = true;  // true ~ tft.tft();
  }


  /* Spieler 1 Paddel */
  tft.drawFastVLine(PLAYER_ONE_X, playerOne_y, PADDLE_HEIGHT, Black);
  if (UP_STATE) {
    playerOne_y = playerOne_y - 1;
  }
  if (DOWN_STATE) {
    playerOne_y = playerOne_y + 1;
  }
  UP_STATE = DOWN_STATE = false;


  if (playerOne_y < 1)
    playerOne_y = 1;  // Um nicht aus dem Bildschirm zu gelangen
  if (playerOne_y + PADDLE_HEIGHT > SCREEN_HEIGHT)
    playerOne_y = SCREEN_HEIGHT - PADDLE_HEIGHT;
  tft.drawFastVLine(PLAYER_ONE_X, playerOne_y, PADDLE_HEIGHT, Red);


  update = true;


  /* Spieler 2 Paddel */
  tft.drawFastVLine(PLAYER_TWO_X, playerTwo_y, PADDLE_HEIGHT, Black);
  if (UP_STATE_TWO) {
    playerTwo_y -= 1;  // Paddle 1 nach unten
  }
  if (DOWN_STATE_TWO) {
    playerTwo_y += 1;  // Paddle 1 nach oben
  }
  UP_STATE_TWO = DOWN_STATE_TWO = false;


  if (playerTwo_y < 1)
    playerTwo_y = 1;  // Um nicht aus dem Bildschirm zugelangen
  if (playerTwo_y + PADDLE_HEIGHT > SCREEN_HEIGHT)
    playerTwo_y = SCREEN_HEIGHT - PADDLE_HEIGHT;
  tft.drawFastVLine(PLAYER_TWO_X, playerTwo_y, PADDLE_HEIGHT, Blue);
}
