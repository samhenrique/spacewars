#include "raylib.h"
#include <math.h>
#include <stdio.h>

struct player {
  Vector2 position;
  float rotation;
  int victories;
};

int rotationShot(int dano, struct player p, struct player ps, Sound laser,
                 int screenWidth, int screenHeight, int rColision, int rDraw,
                 Color color) {
  int cpositionx = p.position.x + 60;
  int cpositiony = p.position.y;
  int cpositionx2 = p.position.x + 10;
  int cpositiony2 = p.position.y;
  PlaySoundMulti(laser);
  if (p.rotation == 270.0) {
    while (cpositionx <= screenWidth) {
      cpositionx2 += 50;
      cpositiony2 = p.position.y;
      Vector2 circlePosition = (Vector2){cpositionx, cpositiony};
      if (CheckCollisionCircles(circlePosition, rColision, ps.position, 28.0)) {
        dano += 1;
        break;
      }
      DrawCircle(cpositionx2, cpositiony2, rDraw, color);
      cpositionx += 1;
    }
  } else if (p.rotation == 360.0 || p.rotation == 0.0) {
    ps.position.x += 60;
    while (cpositiony <= screenHeight) {
      cpositionx2 = p.position.x;
      cpositiony2 += 50;
      DrawCircle(cpositionx - 60, cpositiony2, rDraw, color);
      Vector2 circlePosition = (Vector2){cpositionx, cpositiony};
      if (CheckCollisionCircles(circlePosition, rColision, ps.position, 28.0)) {
        dano += 1;
        break;
      }
      cpositiony += 1;
    }
  } else if (p.rotation == 90.0) {
    while (cpositionx >= 0) {
      cpositionx2 -= 50;
      cpositiony2 = p.position.y;
      DrawCircle(cpositionx2, cpositiony, rDraw, color);
      Vector2 circlePosition = (Vector2){cpositionx, cpositiony};
      if (CheckCollisionCircles(circlePosition, rColision, ps.position, 28.0)) {
        dano += 1;
        break;
      }
      cpositionx -= 1;
    }
  } else if (p.rotation == 180.0) {
    ps.position.x += 60;
    while (cpositiony >= 0) {
      cpositionx2 = p.position.x;
      cpositiony2 -= 50;
      DrawCircle(cpositionx - 60, cpositiony2, rDraw, color);
      Vector2 circlePosition = (Vector2){cpositionx, cpositiony};
      if (CheckCollisionCircles(circlePosition, rColision, ps.position, 28.0)) {
        dano += 1;
        break;
      }
      cpositiony -= 1;
    }
  } else if (p.rotation == 315.0) {
    cpositionx = cpositionx2;
    cpositiony = cpositiony2;
    while ((cpositionx <= screenWidth) || (cpositiony <= screenHeight)) {
      cpositiony2 += 50.0;
      cpositionx2 += 50.0;
      cpositionx += 1;
      cpositiony += 1;
      Vector2 circlePosition = (Vector2){cpositionx, cpositiony};
      DrawCircle(cpositionx2, cpositiony2, rDraw, color);
      if (CheckCollisionCircles(circlePosition, rColision, ps.position, 28.0)) {
        dano += 1;
        break;
      }
    }
  } else if (p.rotation == 45.0) {
    cpositionx = cpositionx2;
    cpositiony = cpositiony2;
    while ((cpositionx >= 0) || (cpositiony <= screenHeight)) {
      cpositiony2 += 50.0;
      cpositionx2 -= 50.0;
      cpositionx -= 1;
      cpositiony += 1;
      Vector2 circlePosition = (Vector2){cpositionx, cpositiony};
      DrawCircle(cpositionx2, cpositiony2, rDraw, color);
      if (CheckCollisionCircles(circlePosition, rColision, ps.position, 28.0)) {
        dano += 1;
        break;
      }
    }
  } else if (p.rotation == 135.0) {
    cpositionx = cpositionx2;
    cpositiony = cpositiony2;
    while ((cpositiony >= 0) || (cpositionx >= 0)) {
      cpositiony2 -= 50.0;
      cpositionx2 -= 50.0;
      cpositionx -= 1;
      cpositiony -= 1;
      Vector2 circlePosition = (Vector2){cpositionx, cpositiony};
      DrawCircle(cpositionx2, cpositiony2, rDraw, color);
      if (CheckCollisionCircles(circlePosition, rColision, ps.position, 28.0)) {
        dano += 1;
        break;
      }
    }
  } else if (p.rotation == 225.0) {
    cpositionx = cpositionx2;
    cpositiony = cpositiony2;
    while ((cpositiony >= 0) || (cpositionx <= screenWidth)) {
      cpositiony2 -= 50.0;
      cpositionx2 += 50.0;
      cpositionx += 1;
      cpositiony -= 1;
      Vector2 circlePosition = (Vector2){cpositionx, cpositiony};
      DrawCircle(cpositionx2, cpositiony2, rDraw, color);
      if (CheckCollisionCircles(circlePosition, rColision, ps.position, 28.0)) {
        dano += 1;
        break;
      }
    }
  }
  return dano;
}
Vector2 returnToMap(struct player p, int screenWidth, int screenHeight) {
  if (p.position.y > screenHeight)
    return p.position = (Vector2){p.position.x, 0};
  if (p.position.y < 0)
    return p.position = (Vector2){p.position.x, screenHeight};
  if (p.position.x > screenWidth)
    return p.position = (Vector2){0, p.position.y};
  if (p.position.x < 0)
    return p.position = (Vector2){screenWidth, p.position.y};
}
Vector2 movimentation(int keyLeft, int keyRight, int keyUp, int keyDown,
                      struct player p) {
  if (IsKeyDown(keyLeft)) {
    p.position.x += 5.0f;
  }
  if (IsKeyDown(keyRight)) {
    p.position.x -= 5.0f;
  }
  if (IsKeyDown(keyUp)) {
    p.position.y -= 5.0f;
  }
  if (IsKeyDown(keyDown)) {
    p.position.y += 5.0f;
  }
  return p.position = (Vector2){p.position.x, p.position.y};
}
float rotation(struct player p, int clockwise, int counterClockwise) {
  if (IsKeyPressed(clockwise)) { // quando a rotacao chega em 360° ela ao enves
                                 // de somar mais 45 e ir pra segunda volta do
                                 // circulo, ela volta para o 0
    p.rotation += 45.0;
    if (p.rotation == 360) {
      p.rotation = 0.0;
    }
    return p.rotation;
  }
  if (IsKeyPressed(counterClockwise)) { // quando o valor fica negativo, a
                                        // rotacao tem que voltar pro 315.0 par
                                        // nao ficar um angulo negativo
    p.rotation -= 45.0;
    if (p.rotation == -45.0) {
      p.rotation = 315.0;
    }
    return p.rotation;
  }
  return p.rotation;
}

