void screenWash(){
  display.clearDisplay();
  for (int y = 0; y < SCREEN_HEIGHT / 8; y++) 
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      display.setCursor(x * 6, y * 8);
      display.print(random(2)); // Print a random 0 or 1
      display.display();
    }
  }

