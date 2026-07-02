#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define BUTTON_PIN 3
#define BUZZER_PIN 4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Dino properties
int dinoX = 10;
int dinoY = 40;
int dinoWidth = 10;
int dinoHeight = 10;
int velocity = 0;
bool isJumping = false;

// Obstacle properties
int cactusX = SCREEN_WIDTH;
int cactusY = 40;
int cactusWidth = 8;
int cactusHeight = 10;

// Game state
int score = 0;
bool gameOver = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Internal pull-up enabled
  pinMode(BUZZER_PIN, OUTPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  if (!gameOver) {
    // Handle jump (LOW means button is pressed)
    if (digitalRead(BUTTON_PIN) == LOW && !isJumping) {
      velocity = -12;
      isJumping = true;
      tone(BUZZER_PIN, 1000, 100); // Jump sound
    }

    // Gravity and Dino position
    dinoY += velocity;
    velocity += 1;
    if (dinoY >= 40) {
      dinoY = 40;
      velocity = 0;
      isJumping = false;
    }

    // Move cactus
    cactusX -= 3;
    if (cactusX + cactusWidth < 0) {
      cactusX = SCREEN_WIDTH;
      score++;
    }

    // Collision detection
    if (cactusX < dinoX + dinoWidth &&
        cactusX + cactusWidth > dinoX &&
        dinoY + dinoHeight >= cactusY) {
      tone(BUZZER_PIN, 200, 500); // Game over sound
      gameOver = true;
    }

    // Draw game
    display.clearDisplay();
    display.fillRect(dinoX, dinoY, dinoWidth, dinoHeight, SSD1306_WHITE); // Dino
    display.fillRect(cactusX, cactusY, cactusWidth, cactusHeight, SSD1306_WHITE); // Cactus
    display.setCursor(0, 0);
    display.print("Score: ");
    display.print(score);
    display.display();

    delay(30);
  } else {
    // Game Over screen
    display.clearDisplay();
    display.setCursor(30, 25);
    display.println("GAME OVER!");
    display.setCursor(30, 40);
    display.print("Score: ");
    display.print(score);
    display.display();

    // Wait for button to restart
    if (digitalRead(BUTTON_PIN) == LOW) {
      gameOver = false;
      cactusX = SCREEN_WIDTH;
      score = 0;
      dinoY = 40;
      velocity = 0;
      isJumping = false;
      delay(500); // debounce
    }
  }
}
