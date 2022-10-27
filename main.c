#include "funcoes.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#define MAX_FRAME_SPEED 15
#define MIN_FRAME_SPEED 1
#include "save.h"
#include "definition.h"

int main() {
  FILE *save;
  SetTargetFPS(30);
  int screenWidth = 1000;
  int screenHeight = 650;
  int menu = 0;
  int p1Dano = 0;
  int p2Dano = 0;

  InitWindow(screenWidth, screenHeight, "sky war");

  Texture2D space = LoadTexture("resources/space.png");
  Texture2D backinit = LoadTexture("resources/backgroundinicial.png");
  Texture2D s1 = LoadTexture("resources/p1.png");
  Texture2D s2 = LoadTexture("resources/p2.png");
  Texture2D met = LoadTexture("resources/met.png");
  Texture2D met2 = LoadTexture("resources/meteor2.png");

  Vector2 position = {0.0f, 0.0f};
  Rectangle frameRec = {0.0f, 0.0f, (float)space.width, (float)space.height};

  int currentFrame = 0;
  int framesCounter = 0;
  int framesSpeed = 4;

  InitAudioDevice();
  Sound laser = LoadSound("resources/laser.ogg");
  SetSoundVolume(laser, 0.5f);

  Music musicGame = LoadMusicStream("resources/sound-track.mp3");
  musicGame.looping = true;
  SetMusicVolume(musicGame, 0.5f);

  Music musicMenu = LoadMusicStream("resources/menuSound.mp3");
  musicMenu.looping = true;
  SetMusicVolume(musicMenu, 0.5f);

  struct player p1, p2, meteor, meteor2; // criacao
  p1.position = (Vector2){screenWidth / 16, screenHeight / 8};
  p1.rotation = 270.0;
  p1.victories = 0;
  p2.position = (Vector2){screenWidth * 15 / 16, screenHeight - 100};
  p2.rotation = 90.0;
  p2.victories = 0;
  meteor.position = (Vector2){GetRandomValue(0, screenWidth),
                              GetRandomValue(0, screenHeight)};
  meteor2.position = (Vector2){GetRandomValue(0, screenWidth),
                               GetRandomValue(0, screenHeight)};

  Rectangle shipRectangle = {
      0, 0, s1.width,
      s1.height}; // servem para conseguir pegar o centro das naves
  Vector2 shipCenter = {s1.width / 2, s1.height / 2};
  Rectangle meteorRectangle = {0, 0, met.width, met.height};
  Rectangle meteor2Rectangle = {0, 0, met2.width, met2.height};
  Vector2 meteorCenter = {met.width / 2, met.height / 2};
  Vector2 meteor2Center = {met2.width / 2, met2.height / 2};

  while (!WindowShouldClose()) {
    DrawTexture(backinit, 0, 0, WHITE);
    UpdateMusicStream(musicMenu);
    PlayMusicStream(musicMenu);

    if (menu == 0) {
      if (IsKeyPressed(KEY_ENTER)) {
        menu = 1;
      }
      if (IsKeyPressed(KEY_TAB)) {
        menu = 2;
      }
      if(p1Dano>100 || p2Dano>100){
          p1.position.x = p1x0;
          p1.position.y = p1y0;
          p2.position.x = p2x0;
          p2.position.y = p2y0;
          p1.rotation = p1r0;
          p2.rotation = p2r0;
          p1Dano = p1D0;
          p2Dano = p2D0;
          meteor.position.x = m1x0;
          meteor.position.y = m1y0;
          meteor2.position.x = m2x0;
          meteor2.position.y = m2y0;
          SetTargetFPS(30);
      }
    }

    if (menu == 2) {
      p1.position.x = p1x;
      p1.position.y = p1y;
      p2.position.x = p2x;
      p2.position.y = p2y;
      p1.rotation = p1r;
      p2.rotation = p2r;
      p1Dano = p1D;
      p2Dano = p2D;
      meteor.position.x = m1x;
      meteor.position.y = m1y;
      meteor2.position.x = m2x;
      meteor2.position.y = m2y;
      p1.victories = p1v;
      p2.victories = p2v;
      menu = 1;
    }

    if (menu == 1) {
      UpdateMusicStream(musicGame);
      StopMusicStream(musicMenu);

      framesCounter++;
      if (framesCounter >= (30 / framesSpeed)) {
        framesCounter = 0;
        currentFrame++;
        if (currentFrame > 5)
          currentFrame = 0;
        frameRec.x = (float)currentFrame * (float)space.width / 6;
      }

      BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawTextureRec(space, frameRec, position, WHITE);
      PlayMusicStream(musicGame);
      // desenhando as navese o meteoro na tela, fazendo movimentacao e rotacao
      DrawTexturePro(s1, shipRectangle,
                     (Rectangle){p1.position.x, p1.position.y,
                                 shipRectangle.width, shipRectangle.height},
                     shipCenter, p1.rotation, WHITE);
      DrawTexturePro(s2, shipRectangle,
                     (Rectangle){p2.position.x, p2.position.y,
                                 shipRectangle.width, shipRectangle.height},
                     shipCenter, p2.rotation, WHITE);
      DrawTexturePro(met2, meteor2Rectangle,
                     (Rectangle){meteor2.position.x, meteor2.position.y,
                                 meteor2Rectangle.width,
                                 meteor2Rectangle.height},
                     meteor2Center, 0.0, WHITE);
      DrawTexturePro(met, meteorRectangle,
                       (Rectangle){meteor.position.x, meteor.position.y,
                                   meteorRectangle.width,
                                   meteorRectangle.height},
                       meteorCenter, 0.0, WHITE); 
      
      DrawText(TextFormat("blue wins : %i", p1.victories), 850, 50,20, BLUE);
      DrawText(TextFormat("red wins %i", p2.victories), 850,80, 20, RED);


      
      p1.position = movimentation(KEY_D, KEY_A, KEY_W, KEY_S, p1);
      p1.position = returnToMap(p1, screenWidth, screenHeight);
      p1.rotation = rotation(p1, KEY_R, KEY_E);
      p2.position = movimentation(KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN, p2);
      p2.rotation = rotation(p2, KEY_J, KEY_K);
      p2.position = returnToMap(p2, screenWidth, screenHeight);
      
      meteor.position = returnToMap(meteor, screenWidth, screenHeight);
      
      meteor2.position = returnToMap(meteor2, screenWidth, screenHeight);

      if (CheckCollisionCircles(meteor.position, 15, p1.position, 28.0)) {
         p2Dano += 1;
      }      
      if (CheckCollisionCircles(meteor.position, 15, p2.position, 28.0)) {
        p1Dano += 1;
      }
      if (CheckCollisionCircles(meteor2.position, 35, p1.position, 28.0)) {
        p2Dano += 1;
      }
      if (CheckCollisionCircles(meteor2.position, 35, p2.position, 28.0)) {
        p1Dano += 1;
      }
      
      meteor.position.x += 2;
      meteor.position.y += 2;
      meteor2.position.x -= 2;
      meteor2.position.y += 2;

      // chamada das funcoes de tiro.
      if (IsKeyPressed(KEY_SPACE) && p1Dano <= 60)
        p1Dano = rotationShot(p1Dano, p1, p2, laser, screenWidth, screenHeight,
                              1, 5, BLUE);
      if (IsKeyPressed(KEY_SPACE) && p1Dano > 60)
        p1Dano = rotationShot(p1Dano, p1, p2, laser, screenWidth, screenHeight,
                              7, 10, BLUE);
      if (IsKeyPressed(KEY_L) && p2Dano <= 60)
        p2Dano = rotationShot(p2Dano, p2, p1, laser, screenWidth, screenHeight,
                              1, 5, RED);
      if (IsKeyPressed(KEY_L) && p2Dano > 60)
        p2Dano = rotationShot(p2Dano, p2, p1, laser, screenWidth, screenHeight,
                              7, 10, RED);
                              
      
      DrawText(TextFormat("%i", p1Dano), screenWidth / 4, screenHeight / 32,
               100, BLUE);
      DrawText(TextFormat("%i", p2Dano), screenWidth * 2.75 / 4,
               screenHeight / 32, 100, RED);
      
      if (IsKeyPressed(KEY_TAB)) {
        save = fopen("save.h", "w");
        fprintf(save,
                "float p1x = %f; \nfloat p1y = %f; \nfloat p2x = %f; \nfloat "
                "p2y = %f; \nfloat p1r = %f; \nfloat p2r = %f; \nint p1D = %i; "
                "\nint p2D = %i; \nfloat m1x = %f;\nfloat m1y = %f;\nfloat m2x "
                "= %f;\nfloat m2y = %f; \nint p1v = %i; \nint p2v = %i;",
                p1.position.x, p1.position.y, p2.position.x, p2.position.y,
                p1.rotation, p2.rotation, p1Dano, p2Dano, meteor.position.x,
                meteor.position.y, meteor2.position.x, meteor2.position.y,p1.victories,p2.victories);
        fclose(save);
      }
    } 
    if(p1Dano > 100){                            
        save = fopen("save.h", "w");
        DrawRectangle(0, 325, 1000, 70, WHITE);
        DrawText("Player Blue Wins", 320, 335, 50, BLUE);
        p1.victories += 1;
        save = fopen("save.h", "w");
        fprintf(save,
                "float p1x = 62.000000; \nfloat p1y = 325.000000; \nfloat p2x = 937.000000; \nfloat "
                "p2y = 325.000000; \nfloat p1r = 270.000000;  \nfloat p2r = 90.000000;  \nint p1D = 0; "
                "\nint p2D = 0; \nfloat m1x = %f;\nfloat m1y = %f;\nfloat m2x "
                "= %f;\nfloat m2y = %f; \nint p1v = %i; \nint p2v = %i;",
                meteor.position.x,meteor.position.y, meteor2.position.x, meteor2.position.y,p1.victories,p2.victories);                
        fclose(save);
        fclose(save);
        SetTargetFPS(1);
        menu = 0;                       
    } 
    if(p2Dano > 100){
        save = fopen("save.h", "w");
        DrawRectangle(0, 325, 1000, 70, WHITE);
        DrawText("Player Red Wins", 320, 335, 50, RED);
        p2.victories += 1;
        save = fopen("save.h", "w");
        fprintf(save,
                "float p1x = 62.000000; \nfloat p1y = 325.000000; \nfloat p2x = 937.000000; \nfloat "
                "p2y = 325.000000; \nfloat p1r = 270.000000;  \nfloat p2r = 90.000000;  \nint p1D = 0; "
                "\nint p2D = 0; \nfloat m1x = %f;\nfloat m1y = %f;\nfloat m2x "
                "= %f;\nfloat m2y = %f; \nint p1v = %i; \nint p2v = %i;",
                meteor.position.x,meteor.position.y, meteor2.position.x, meteor2.position.y,p1.victories,p2.victories);                
        fclose(save);
        fclose(save);
        SetTargetFPS(1);
        menu = 0; 
    }    
    EndDrawing(); 
       
  }
  StopSoundMulti();
  UnloadSound(laser);
  UnloadMusicStream(musicGame);
  UnloadMusicStream(musicMenu);
  CloseAudioDevice();
  UnloadTexture(backinit);
}

