/* 
  WIRING
    - Connect Channel Switching button @ Digital pin 2
    - Connect Channel Value Up button @ Digital pin 3
    - Connect Channel Value Down button @ Digital pin 4

    - Connect LED red at pin 9
    - Connect LED green at pin 10
    - Connect LED blue at pin 11
*/

struct Button {
  int pin;
  bool released;
};

struct Button btnChannel = {2, true};
struct Button btnUp = {3, true};
struct Button btnDown = {4, true};


namespace LED {
  const int RED = 9;
  const int GREEN = 10;
  const int BLUE = 11;

  int red = 0;
  int green = 0;
  int blue = 0;

  int channel = 0;
};

const int INTERVAL = 25; // Constant of how many steps to increment/ decrement by


void RGB(int, int, int);
void checkReleased();
void incChannel(int *, const int);

void listenForChannel();
void listenForUp();

void updateInterface(int, int, int);

void setup() {
  Serial.begin(9600);
  pinMode(btnChannel.pin, INPUT);
  pinMode(btnUp.pin, INPUT);
  pinMode(btnDown.pin, INPUT);

  updateInterface(LED::red, LED::green, LED::blue);
}

void loop () {  
  checkReleased(); // Update Release state

  listenForChannel();
  listenForUp();

  RGB(LED::red, LED::green, LED::blue);
}

void updateInterface(int r, int g, int b) { 
  r = constrain(r, 0, 255);
  g = constrain(g, 0, 255);
  b = constrain(b, 0, 255);

  // Update Interface with the updated colouring
  Serial.print("#");
  Serial.print(r);
  Serial.print(",");
  Serial.print(g);
  Serial.print(",");
  Serial.print(b);
  Serial.print("\n");
}


void checkReleased() {
  if (digitalRead(btnChannel.pin) == LOW) {
    btnChannel.released = true;
  }

  if (digitalRead(btnUp.pin) == LOW) {
    btnUp.released = true;
  }
}

void incChannel(int * color, const int increment) {
  if (*color < 255) {
    *color += increment;
  } else {
    *color = 0;
  }

  *color = constrain(*color, 0, 255);
}

void listenForUp() {
  if (digitalRead(btnUp.pin) == HIGH && btnUp.released) {
    btnUp.released = false;

    switch (LED::channel) {
      case 0:
        incChannel(&LED::red, +INTERVAL);
        break;
      case 1:
        incChannel(&LED::green, +INTERVAL);
        break;
      case 2:
        incChannel(&LED::blue, +INTERVAL);
        break;

    }

    updateInterface(LED::red, LED::green, LED::blue); // Update interface with updated colour
    delay(150); // Delay to prevent accidental clicks
  }
}

void listenForChannel() {
  if (digitalRead(btnChannel.pin) == HIGH && btnChannel.released) {
    btnChannel.released = false;

    int red = 0;
    int green = 0;
    int blue = 0;

    LED::channel += 1;
    if (LED::channel >= 3) {
      LED::channel = 0;
    }

    switch (LED::channel) {
      case 0:
        red = 255;
        green = 0;
        blue = 0;
        break;
      case 1:
        red = 0;
        green = 255;
        blue = 0;
        break;
      case 2:
        red = 0;
        green = 0;
        blue = 255;
        break;
    }

    /*
     * Blinking animation
    */

    delay(150);
    RGB(red,green,blue);
    updateInterface(red,green,blue);

    delay(500);
    RGB(0,0,0);
    updateInterface(0,0,0);

    delay(150);
    RGB(red,green,blue);
    updateInterface(red,green,blue);

    delay(500);
    RGB(0,0,0);
    updateInterface(0,0,0);

    delay(100);
    updateInterface(LED::red, LED::green, LED::blue); // Update the interface again with true color here
    // Prevent continuous sending to buffer, which slows interface
  }
}

void RGB(int r, int g, int b) {
  analogWrite(LED::RED, r);
  analogWrite(LED::GREEN, g);
  analogWrite(LED::BLUE, b);
}
