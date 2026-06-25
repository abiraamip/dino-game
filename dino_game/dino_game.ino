#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define BUTTON_PIN 2

// Dino
int dinoX = 15;
int dinoY = 50;
int dinoSize = 10;

bool jumping = false;
int velocityY = 0;
const int gravity = 1;

// Obstacle
int obstacleX = 128;
int obstacleY = 50;
int obstacleWidth = 8;
int obstacleHeight = 10;

// Score
long score = 0;
bool gameOver = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1);
  }

  display.clearDisplay();
}

void loop() {

  if (gameOver) {
    showGameOver();

    if (digitalRead(BUTTON_PIN) == LOW) {
      delay(300);
      resetGame();
    }

    return;
  }

  // Jump
  if (digitalRead(BUTTON_PIN) == LOW && !jumping) {
    jumping = true;
    velocityY = -10;
  }

  // Physics
  if (jumping) {
    dinoY += velocityY;
    velocityY += gravity;

    if (dinoY >= 50) {
      dinoY = 50;
      jumping = false;
    }
  }

  // Move obstacle
  obstacleX -= 4;

  if (obstacleX < -obstacleWidth) {
    obstacleX = 128;
    score++;
  }

  // Collision detection
  if (dinoX < obstacleX + obstacleWidth &&
      dinoX + dinoSize > obstacleX &&
      dinoY < obstacleY + obstacleHeight &&
      dinoY + dinoSize > obstacleY) {

    gameOver = true;
  }

  drawGame();

  delay(30);
}

void drawGame() {

  display.clearDisplay();

  // Ground
  display.drawLine(0, 60, 127, 60, WHITE);

  // Dino
  display.fillRect(dinoX, dinoY, dinoSize, dinoSize, WHITE);

  // Obstacle
  display.fillRect(
    obstacleX,
    obstacleY,
    obstacleWidth,
    obstacleHeight,
    WHITE
  );

  // Score
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(90, 0);
  display.print(score);

  display.display();
}

void showGameOver() {

  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(10, 15);
  display.print("GAME");

  display.setCursor(10, 35);
  display.print("OVER");

  display.setTextSize(1);
  display.setCursor(80, 25);
  display.print("Score:");
  display.print(score);

  display.display();
}

void resetGame() {

  dinoY = 50;
  obstacleX = 128;
  score = 0;
  jumping = false;
  velocityY = 0;
  gameOver = false;
}
