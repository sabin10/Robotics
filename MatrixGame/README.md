## Matrix Game
#### Spaceship

Move through space and avoid the meteors.

Idea is inspired from a previous game that I developed https://github.com/sabin10/spaceship-game.

Developed with Arduino UNO. Homework for [@unibucrobotics](https://www.facebook.com/unibuc.robotics/).

#### Hardware
1. 8x8 LED Matrix -> playing
2. LCD -> menu
3. Joystick -> control
4. MAX7219 Driver -> controlling 8x8 LED Matrix

#### Game Specifications
1. Gets more challenging with time by creating more obstacles and by increasing the speed of their movement.
2. Move the spaceship in order to avoid obstacles.
2. Keeping a score.
3. Maximum 10 levels, every level increases the difficulty.
4. Having 3 lives.
5. Save the highscore in EEPROM(memory).

#### How to Play
1. Press PLAY.
2. Move the spaceship (.:. from the LED Matrix) on X-axis to avoid the meteors that are coming from up.
3. The game is over when the player remains without lives, i.e. it was hitted by 3 meteors.
4. Go to Main Menu and start again.


#### Images
[![](http://img.youtube.com/vi/UjKxxA-Abv4/0.jpg)](https://youtu.be/UjKxxA-Abv4 "")

![](https://i.imgur.com/339jJ87.png)
