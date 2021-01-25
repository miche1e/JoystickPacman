#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Joypad :)
#define VRx 12
#define VRy 14
#define SW 27

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Special chars
byte pacman[8] = {
  B00000,
  B00000,
  B01110,
  B11011,
  B11100,
  B01110,
  B00000,
  B00000
};

byte pacmanBack[8] = {
  B00000,
  B00000,
  B01110,
  B11011,
  B00111,
  B01110,
  B00000,
  B00000
};

byte ghost[8] = {
  B00000,
  B00000,
  B01110,
  B10101,
  B11111,
  B11111,
  B10101,
  B00000
};

byte point[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00100,
  B00000,
  B00000,
  B00000
};

byte eaten[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

// Point Matrix
int points[16][2];
// On screen position
int posX = -1;
int posY = 0;

float ghostX = 15;
float ghostY = 1;
// Pacman direction
bool pacDir = true;
// Pacman special char orientation
int pac = 1;

// Score
int score = 132;

void setup()
{
  //Serial.begin(115200);

  // initialize the LCD
  lcd.begin();

  //Chars initialization
  lcd.createChar(0, point);
  lcd.createChar(1, pacman);
  lcd.createChar(2, pacmanBack);
  lcd.createChar(3, ghost);
  lcd.createChar(4, eaten);

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("PAC-MAN!");
  delay(2000);
  lcd.clear();
  firstSettingPoint();
}

void loop()
{
  settingPoint();

  // Joystick mapping
  int x = map(analogRead(VRx), 0, 4095, -100, 100);
  int y = map(analogRead(VRy), 0, 4095, -100, 100);

  //Joystick direction control
  setDir(x, y);

  // Move component
  movePac();

  // Screen printing
  lcd.setCursor(posX, posY);
  points[posX][posY] = 4;
  if (pacDir) {
    pac = 1;
  } else {
    pac = 2;
  }
  lcd.write(pac);

  lcd.setCursor(ghostX, ghostY);
  lcd.write(3);

  // End Cycle
  delay(300);

  moveGhost();

  gameCheck();

  lcd.clear();

  score--;

  winControl();
}

// Direction control function
void setDir(int x, int y) {
  if (x > 25) {
    pacDir = true;
  }
  if (x < -25) {
    pacDir = false;
  }
  if (y > 25) {
    posY = 1;
  }
  if (y < -25) {
    posY = 0;
  }
}

void movePac() {
  if (pacDir) {
    if (posX == 15) {
      posX = -1;
    }
    posX ++;
  } else {
    if (posX == 0) {
      posX = 16;
    }
    posX --;
  }
}

void moveGhost() {
  if (ghostX < posX) {
    ghostX += 0.5;

  } else if (ghostX > posX) {
    ghostX -= 0.5;
  }
  ghostY = posY;
}

void gameCheck() {
  if (posX == ghostX && posY == ghostY) {
    lcd.setCursor(3, 0);
    lcd.print("YOU LOST!");
    delay(3000);
    startNew();
  }
}

void firstSettingPoint() {
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < 16; i++) {
      points[i][j] = 0;
      lcd.setCursor(i, j);
      lcd.write(0);
    }
  }
}

void settingPoint() {
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < 16; i++) {
      lcd.setCursor(i, j);
      lcd.write(points[i][j]);
    }
  }
}

void winControl() {
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < 16; i++) {
      if (points[i][j] == 0) {
        return;
      }
    }
  }
  lcd.setCursor(4, 0);
  lcd.print("YOU WIN!!");
  lcd.setCursor(3, 1);
  lcd.print("Score: " + String(score));
  delay(7000);
  lcd.clear();
  startNew();
}

void startNew() {
  posX = -1;
  posY = 0;
  ghostX = 15;
  ghostY = 1;
  pacDir = true;
  score = 132;
  lcd.setCursor(4, 1);
  lcd.print("New game..");
  delay(3000);
  lcd.clear();

  lcd.setCursor(4, 0);
  lcd.print("PAC-MAN!");
  delay(2000);
  lcd.clear();
  firstSettingPoint();
}
