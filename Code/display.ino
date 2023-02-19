#include <TFT_eSPI.h>

//create the screen variable from the library
TFT_eSPI tft = TFT_eSPI();

// Set up variables for the cursor and counter. Cursor starts in the middle of the screen.
float cursorX = 240.0;
float cursorY = 160.0;
int resetCount = 0;

// Setting the joystick pins here so we can easily change them
#define JOYSTICK_X_PIN A0
#define JOYSTICK_Y_PIN A1
#define JOYSTICK_BUTTON_PIN 46

// Define colours in 4-digit hex                                  
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define DARK_GREEN 0x0B60
#define GRASS_LIGHT_GREEN 0x0660
#define GRASS_DARK_GREEN 0x0500


// Board Varaibles
const int BOARD_SIZE = 312;
const int NUM_OF_TILES = 8;
const int TILE_SIZE = BOARD_SIZE / NUM_OF_TILES;
const int START_X = 84;
const int START_Y = 4;
const int apple_radius = TILE_SIZE/3.0;

const int white_tile = 10;
const int black_tile = 9;

const int score = 0;

int snakeMap[NUM_OF_TILES][NUM_OF_TILES];
int snakeX[NUM_OF_TILES];
int snakeY[NUM_OF_TILES];



void setup() {

  // Initalize the screen and set the orientation correctly, then make sure it's clear.
  Initialize_Screen_and_Board();
}

void loop(){
 Serial.println("TEST");
}



void createBoard() {
  //Board of the snake game
  //The board will go here, assuming 8x8
  for (int i = 0; i < NUM_OF_TILES; i++) {
    for (int j = 0; j < NUM_OF_TILES; j++) {
      if(((i + j) % 2) == 0) { //Black Tile 9 
        snakeMap[i][j] = black_tile;
      } else { //White Tile 10
        snakeMap[i][j] = white_tile;
      }
    }
  }
}


void displayBoard() {
  //This will display the whole board on the Arduino SPI LCD
  for (int i = 0; i < NUM_OF_TILES; i++) {
    for (int j = 0; j < NUM_OF_TILES; j++) {
      if(snakeMap[i][j] == black_tile) {//Black Tile 9
        tft.drawRect(TILE_SIZE*i + START_X, TILE_SIZE*j + START_Y, TILE_SIZE, TILE_SIZE, GRASS_DARK_GREEN);
        tft.fillRect(TILE_SIZE*i + START_X, TILE_SIZE*j + START_Y, TILE_SIZE, TILE_SIZE, GRASS_DARK_GREEN);
      } else { //White Tile 10
        tft.drawRect(TILE_SIZE*i + START_X, TILE_SIZE*j + START_Y, TILE_SIZE, TILE_SIZE, GRASS_LIGHT_GREEN);
        tft.fillRect(TILE_SIZE*i + START_X, TILE_SIZE*j + START_Y, TILE_SIZE, TILE_SIZE, GRASS_LIGHT_GREEN);
      }
    }
  }
}


void Spawn_Snake(int x_tile_right, int y_tile_down){
  //Spawns green rectangle on (x_tile_right + 1, y_tile_down + 1) square on board
  int snake_spawn_x = START_X + (x_tile_right* TILE_SIZE);
  int snake_spawn_y = START_Y + (y_tile_down * TILE_SIZE);
  
  tft.fillRect(snake_spawn_x, snake_spawn_y, TILE_SIZE, TILE_SIZE, MAGENTA);

}

void Display_Apple(int x_tile_right, int y_tile_down){
  
  //made it a bit smaller so it doesn't conflict with the border
  float origin_x = START_X + (x_tile_right*TILE_SIZE) + TILE_SIZE/2.0;
  float origin_y = START_Y + (y_tile_down*TILE_SIZE) + TILE_SIZE/2.0;

  float leaf_xo = origin_x;
  float leaf_yo = origin_y - apple_radius;
  float leaf_x = leaf_xo + apple_radius;
  float leaf_y = leaf_yo - 3;
  
  tft.fillCircle(origin_x, origin_y, apple_radius, RED);
  tft.drawLine(leaf_xo, leaf_yo, leaf_x, leaf_y, DARK_GREEN);
  tft.drawLine(leaf_xo, leaf_yo, leaf_x-1, leaf_y-1, DARK_GREEN);
  tft.drawLine(leaf_xo, leaf_yo, leaf_x+1, leaf_y+1, DARK_GREEN);
  tft.drawLine(leaf_xo, leaf_yo, leaf_x-2, leaf_y-2, DARK_GREEN);
 
}

void Display_Score_Screen(int score){
  Display_Apple(-2,0);

  String text = String(score);
  tft.setTextSize(2);

//change start position if double digit to look better
  if(score < 10){
  tft.drawString(text, 2*apple_radius, apple_radius+35);
  }
  else{
    tft.drawString(text, apple_radius, apple_radius+35);
  }
}

void Initialize_Screen_and_Board(){
  //Makes background black
  tft.begin();
  tft.setRotation(1); //Makes starting point at top left corner of the screen when its horizontal
  tft.fillScreen(BLACK);
  createBoard();
  displayBoard();
  Spawn_Snake(3,3);   //Starts snake at (4,4) square on board
  Display_Score_Screen(score); //Display score of 0 
}
  