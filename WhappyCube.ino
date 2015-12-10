// Lisa Walkosz-Migliacio Whappy Cube code

#include <Adafruit_FloraPixel.h>
#include <Adafruit_NeoPixel.h>
#include <CapacitiveSensor.h>

// conductive fabric inputs
CapacitiveSensor   _D10 = CapacitiveSensor(9,10); // P1 Yellow
CapacitiveSensor   _SDA = CapacitiveSensor(9,2); // P1 Green
CapacitiveSensor   _D12 = CapacitiveSensor(9,12); // P2 Yellow
CapacitiveSensor   _RX = CapacitiveSensor(9,0); // P2 Green

// the LED array, we have 10 LEDs
// player1 (orange) is 5-10
// player2 (purple) is 0-4
Adafruit_FloraPixel strip = Adafruit_FloraPixel(10);

// some colors we can make!
enum colors {
  RED = 0,
  ORANGE = 1,
  YELLOW = 2,
  GREEN = 3,
  BLUE = 4,
  PURPLE = 5,
  PINK = 8,
  AQUA = 9,
  LIME = 10,
  GRAY = 11,
  WHITE = 6,
  LOW_WHITE = 7
};

// how many players
enum players {
  NONE = 0,
  P1 = 1,
  P2 = 2
};

// state variables
enum state {
  GET_READY = 0,
  WAIT_FOR_GAME = 1,
  SHOW_ANSWER = 2,
  SHOW_WINNER = 3,
  END_GAME = 4
};

int STATE = GET_READY;

// checking user input
bool p1YellowStart = false;
bool p1GreenStart = false;
bool p2YellowStart = false;
bool p2GreenStart = false;
bool p1Ready = false;
bool p2Ready = false;

// when we are ready to pick a winner, which color wins, 
// which player wins, and how many they won
int pickColor;
int winner = NONE;
int p1Wins = 0;
int p2Wins = 0;

void setup() {
   //D10.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   //RX.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   //TX.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   //D12.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   
   Serial.begin(9600);

   // we'll need this random seed because of picking either
   // green or yellow colors
   randomSeed(analogRead(0));
   strip.begin();
   strip.show();
}

void loop() {
  if (STATE == GET_READY) {
    Rainbow(3);
    ColorAll(RED);
  }
  if (STATE == WAIT_FOR_GAME) {
    delay(10); 
    if (!p1YellowStart && touchP1Yellow()) {
      Start(P1, YELLOW);
      p1YellowStart = true;
    }
    if (!p1GreenStart && touchP1Green()) {
      Start(P1, GREEN);
      p1GreenStart = true;
    }
    if (!p2YellowStart && touchP2Yellow()) {
      Start(P2, YELLOW);
      p2YellowStart = true;
    }
    if (!p2GreenStart && touchP2Green()) {
      Start(P2, GREEN);
      p2GreenStart = true;
    }
    if (p1YellowStart && p1GreenStart) {
      delay(50);
      Start(P1, ORANGE);
      p1Ready = true;
    }
    if (p2YellowStart && p2GreenStart) {
      delay(50);
      Start(P2, PURPLE);
      p2Ready = true;
    }
    if (p1Ready && p2Ready) {
      delay(1000);
      ColorAll(LOW_WHITE);
      delay(1000);
      ColorAll(RED);
      delay(1000);
      ColorAll(BLUE);
      delay(1000);
      ColorAll(RED);
      delay(1000);

      // pick a random color between yellow and green
      int randomNum = random(0, 2);
      if (randomNum == 0) {
        pickColor = GREEN;
        ColorAll(GREEN);
      }
      else {
        pickColor = YELLOW;
        ColorAll(YELLOW);
      }
      STATE = SHOW_ANSWER;
    }
  }
  if (STATE == SHOW_ANSWER) {
    delay(10); 
    if (winner == NONE && touchP1Yellow()) {
      if (pickColor == YELLOW) {
        winner = P1;
        p1Wins++;
      }
      else {
        winner = P2;
        p2Wins++;
      }
    }
    if (winner == NONE && touchP1Green()) {
      if (pickColor == GREEN) {
         winner = P1;
         p1Wins++;
      }
      else {
        winner = P2;
        p2Wins++;
      }
    }
    if (winner == NONE && touchP2Yellow()) {
      if (pickColor == YELLOW) {
      winner = P2;
      p2Wins++;
      }
      else {
        winner = P1;
        p1Wins++;
      }
    }
    if (winner == NONE && touchP2Green()) {
      if (pickColor == GREEN) {
      winner = P2;
      p2Wins++;
      }
      else {
        winner = P1;
        p1Wins++;
      }
    }
    if (winner != NONE) {
      STATE = SHOW_WINNER;
    }
  }
  if (STATE == SHOW_WINNER) {
    if (winner == P1) {
      ColorAll(ORANGE);
      delay(500);
      ColorAll(LOW_WHITE);
      delay(500);
      ColorAll(ORANGE);
      delay(500);
      ColorAll(LOW_WHITE);
    }
    if (winner == P2) {
      ColorAll(PURPLE);
      delay(500);
      ColorAll(LOW_WHITE);
      delay(500);
      ColorAll(PURPLE);
      delay(500);
      ColorAll(LOW_WHITE);
    }
    delay(500);
    showScore();
    delay(2000);
    if (p2Wins >= 5 || p1Wins >= 5) {
      winner = NONE;
      STATE = END_GAME;
    }
    else {
      winner = NONE;
      STATE = WAIT_FOR_GAME;
    }
  }
  if (STATE == END_GAME) {
    if (p1Wins >= 5) {
      for (int i = 0; i < 5; i++) {
        ColorAll(ORANGE);
        delay(1000); 
        Rainbow(2);
        delay(1000); 
      }
    }
    else if (p2Wins >= 5) {
      for (int i = 0; i < 5; i++) {
        ColorAll(PURPLE);
        delay(1000); 
        Rainbow(2);
        delay(1000); 
      }
    }
    Rainbow(2);
    // time to reset variables so we can play again!
    p1Ready = false;
    p2Ready = false;
    p1YellowStart = false;
    p2YellowStart = false;
    p1GreenStart = false;
    p2GreenStart = false;
    p1Wins = 0;
    p2Wins = 0;
    ColorAll(RED);
    STATE = WAIT_FOR_GAME;
  }
}
  
void Start(int player, int color) {
  if (player == P1) {
    for (int i = 5; i < 10; i++) {
      PixelColor(i, color);
    }
  }
  else if (player == P2) {
    for (int i = 0; i < 5; i++) {
      PixelColor(i, color);
    }
  }
}

void ColorAll(int color) {
  for (int i = 0; i < 10; i++) {
    PixelColor(i, color);
  }
}

void Rainbow(int times) {
  for (int i = 0; i < times; i++) {
    PixelColor(0, RED);
    PixelColor(5, RED);
    PixelColor(1, YELLOW);
    PixelColor(6, YELLOW);
    PixelColor(2, GREEN);
    PixelColor(7, GREEN);
    PixelColor(3, BLUE);
    PixelColor(8, BLUE);
    PixelColor(4, PURPLE);
    PixelColor(9, PURPLE);
    delay(100); 
    PixelColor(1, RED);
    PixelColor(6, RED);
    PixelColor(2, YELLOW);
    PixelColor(7, YELLOW);
    PixelColor(3, GREEN);
    PixelColor(8, GREEN);
    PixelColor(4, BLUE);
    PixelColor(9, BLUE);
    PixelColor(0, PURPLE);
    PixelColor(5, PURPLE);
    delay(100); 
    PixelColor(2, RED);
    PixelColor(7, RED);
    PixelColor(3, YELLOW);
    PixelColor(8, YELLOW);
    PixelColor(4, GREEN);
    PixelColor(9, GREEN);
    PixelColor(5, BLUE);
    PixelColor(0, BLUE);
    PixelColor(6, PURPLE);
    PixelColor(1, PURPLE);
    delay(100); 
    PixelColor(3, RED);
    PixelColor(8, RED);
    PixelColor(4, YELLOW);
    PixelColor(9, YELLOW);
    PixelColor(5, GREEN);
    PixelColor(0, GREEN);
    PixelColor(6, BLUE);
    PixelColor(1, BLUE);
    PixelColor(7, PURPLE);
    PixelColor(2, PURPLE);
    delay(100);
    PixelColor(4, RED);
    PixelColor(9, RED);
    PixelColor(5, YELLOW);
    PixelColor(0, YELLOW);
    PixelColor(6, GREEN);
    PixelColor(1, GREEN);
    PixelColor(7, BLUE);
    PixelColor(2, BLUE);
    PixelColor(8, PURPLE);
    PixelColor(3, PURPLE);
    delay(100); 
  }
  STATE = WAIT_FOR_GAME;
}

bool touchP1Yellow() {
    long totalD10 =  _D10.capacitiveSensor(30);
    if (totalD10 > 0) {
       //ColorAll(YELLOW);
       return true;
    }
}

bool touchP1Green() {
    long totalSDA = _SDA.capacitiveSensor(30);
    if (totalSDA > 0) {
       //ColorAll(PINK);
       return true;
    }
}

bool touchP2Yellow() {
    long totalD12 =  _D12.capacitiveSensor(30);
    if (totalD12 > 0) {
       //ColorAll(BLUE);
       return true;
    }
}

bool touchP2Green() {
    long totalRX =  _RX.capacitiveSensor(30);
        if (totalRX > 0) {
           //ColorAll(PURPLE);
        return true;
    }
}

void showScore() {
  // p1
    for (int i = 5; i < 10; i++) {
      if (p1Wins > (i-5))
        PixelColor(i, ORANGE);
      else 
        PixelColor(i, LOW_WHITE);
    }
  //p2
    for (int i = 0; i < 5; i++) {
      if (p2Wins > i)
        PixelColor(i, PURPLE);
      else
        PixelColor(i, LOW_WHITE);
    }
}

void PixelColor(int index, int color) {
  if (color == RED) {
    strip.setPixelColor(index, Color(0,255,0));
  }
  if (color == BLUE) {
    strip.setPixelColor(index, Color(0,0,255));
  }
  if (color == YELLOW) {
    strip.setPixelColor(index, Color(255,200,0));
  }
  if (color == PURPLE) {
    strip.setPixelColor(index, Color(0,170,255));
  }
  if (color == GREEN) {
    strip.setPixelColor(index, Color(255,0,0));
  }
  if (color == ORANGE) {
    strip.setPixelColor(index, Color(80,255,0));
  }
  if (color == PINK) {
    strip.setPixelColor(index, Color(0,255,170));
  }
  if (color == AQUA) {
    strip.setPixelColor(index, Color(255,0,255));
  }
  if (color == LIME) {
    strip.setPixelColor(index, Color(255,100,50));
  }
  if (color == WHITE) {
    strip.setPixelColor(index, Color(255,255,255));
  }
  if (color == LOW_WHITE) {
    strip.setPixelColor(index, Color(50,50,50));
  }
  if (color == GRAY) {
    strip.setPixelColor(index, Color(10,10,10));
  }
  strip.show();
}

RGBPixel Color(byte r, byte g, byte b) {
  RGBPixel p;
  
  p.red = r;
  p.green = g;
  p.blue = b;
  
  return p;
}
