#include "app/pong.h"

#include "graphics.h"
#include "touch.h"
#include "lang/lang.h"

#define BACKGROUND_COLOR        TFT_BLACK
#define GAME_COLOR              TFT_WHITE

#define GAME_X_LEFT             35
#define GAME_X_RIGHT            DEFAULT_VIEWPORT.width-35
#define GAME_Y_DOWN             35
#define GAME_Y_UP               DEFAULT_VIEWPORT.height-35

#define MARGIN_Y                35
#define MARGIN_X                35

#define BAR_LENGTH              50
#define BAR_HALF_LENGTH         (BAR_LENGTH / 2)

#define BAR_HEIGHT              10
#define BAR_HALF_HEIGHT         (BAR_HEIGHT / 2)

#define SPACE_BETWEEN_BARS      6
#define DASHED_BARS_LENGTH      10

#define SET_VELOCITY            1

#define BALL_RADIUS             9

int bot_score = 0;
int player_score = 0;

int bot_score;
int player_score ;
int end;
int vx;
int vy;
int rcenter;
float cx;      // circle position (set with mouse)
float cy;
float r;      // circle radius
int nbotx;
int size[2];

void setup_game(){
    r=BALL_RADIUS;
    end=0;
    vx=SET_VELOCITY;
    vy=SET_VELOCITY;
    nbotx=0;
    size[1]=GAME_X_RIGHT-GAME_X_LEFT;
    size[2]=GAME_Y_UP-GAME_Y_DOWN;
    cx=size[1]/2+MARGIN_X;
    cY=size[W]/2+MARGIN_Y;
    

}

void App_Pong::print_top_bar(GBatch_t * batch, int previous_x, int new_x) {
    if (new_x == previous_x) {
        return;
    }

    if (new_x > previous_x) {
        batch->fillRectangle(previous_x - BAR_HALF_LENGTH, MARGIN_Y, new_x - previous_x, BAR_HEIGHT, BACKGROUND_COLOR);
    } else {
        batch->fillRectangle(new_x + BAR_HALF_LENGTH, MARGIN_Y, previous_x - new_x, BAR_HEIGHT, BACKGROUND_COLOR);
    }

    //place_x means the center of the bar 
    batch->fillRectangle(new_x - BAR_HALF_LENGTH, MARGIN_Y, BAR_LENGTH, BAR_HEIGHT, GAME_COLOR);
}

void App_Pong::print_bottom_bar(GBatch_t * batch, int previous_x, int new_x) {
    if (new_x == previous_x) {
        return;
    }

    if (new_x > previous_x) {
        batch->fillRectangle(previous_x - BAR_HALF_LENGTH, batch->viewHeight() - MARGIN_Y - BAR_HEIGHT, new_x - previous_x, BAR_HEIGHT, BACKGROUND_COLOR);
    } else {
        batch->fillRectangle(new_x + BAR_HALF_LENGTH, batch->viewHeight() - MARGIN_Y - BAR_HEIGHT, previous_x - new_x, BAR_HEIGHT, BACKGROUND_COLOR);
    }
    //place_x means the center of the bar 
    batch->fillRectangle(new_x - BAR_HALF_LENGTH, batch->viewHeight() - MARGIN_Y - BAR_HEIGHT, BAR_LENGTH, BAR_HEIGHT, GAME_COLOR);
}

void App_Pong::print_field_lines(GBatch_t * batch, uint32_t color) {
    int numberOfLines = (batch->viewWidth() - DASHED_BARS_LENGTH) / (DASHED_BARS_LENGTH + SPACE_BETWEEN_BARS) + 1;
    int offsetX = ((batch->viewWidth() - DASHED_BARS_LENGTH) % (DASHED_BARS_LENGTH + SPACE_BETWEEN_BARS)) / 2;

    for(int i = 0; i < numberOfLines; i++) {
        int x = offsetX + i * (DASHED_BARS_LENGTH+SPACE_BETWEEN_BARS);

        batch->drawLine(x, batch->viewHeight() / 2, x + DASHED_BARS_LENGTH, batch->viewHeight() / 2, color, 2);
    }
    
    batch->drawLine(1, 1, 1, batch->viewHeight(), color, 8);
    batch->drawLine(batch->viewWidth(), 1, batch->viewWidth(), batch->viewHeight(), color, 8);

}

void App_Pong::print_score(GBatch_t * batch, uint32_t color) {

    sprintf(_bot_score, "%d", bot_score);
    batch->drawString(batch->viewWidth() - MARGIN_X, batch->viewHeight()/2 - MARGIN_Y, _bot_score, color, 4, MC_DATUM);
    
    sprintf(_player_score, "%d", player_score);
    batch->drawString(batch->viewWidth() - MARGIN_X, batch->viewHeight()/2 + MARGIN_Y, _player_score, color, 4, MC_DATUM);

}

void App_Pong::print_ball(GBatch_t * batch, int x_center, int y_center,int previous_x,int previous_y, uint32_t color) {
    batch->fillRectangle(previous_x,previous_y,r, BACKGROUND_COLOR);
    batch->fillCircle(x_center, y_center, BALL_RADIUS, color);
}
bool circleRect(float cx, float cy, float radius, float rx, float ry, float rw, float rh) {

  // temporary variables to set edges for testing
  float testX = cx;
  float testY = cy;

  // which edge is closest?
  if (cx < rx)         testX = rx;      // test left edge
  else if (cx > rx+rw) testX = rx+rw;   // right edge
  if (cy < ry)         testY = ry;      // top edge
  else if (cy > ry+rh) testY = ry+rh;   // bottom edge

  // get distance from closest edges
  float distX = cx-testX;
  float distY = cy-testY;
  float distance = sqrt( (distX*distX) + (distY*distY) );

  // if the distance is less than the radius, collision!
  if (distance <= radius) {
    return true;
  }
  return false;
}

void update_game(float cx, float cy, float r, float plx, float botx,int vx, int vy){
    int cxa=cx;
    int cya=cy;
    cxa=cxa+vx;
    cya=cya+vy;
     if (cxa+r>size[1]+MARGIN_X || cxa-r<MARGIN_X){
       
        vx=-vx;
        }
  
    bool colision_player=circleRect(cxa,cya,r,plx-BAR_HALF_LENGTH,BAR_HEIGHT,BAR_LENGTH,BAR_HEIGHT);
    bool colision_bot=circleRect(cxa,cya,r,botx-BAR_HALF_LENGTH,BAR_HEIGHT,BAR_LENGTH,BAR_HEIGHT);
    
  
    if(colision_bot==1 || colision_bot==1){
        vy=-vy;    
    }

     cy=cy+vy;
     cx=cx+vx;

     if(cy-r>size[2] || cy+r<0)
     end=1;

     return;
        
}

App_Pong::App_Pong() : App(MSG_PONG_NAME, MSG_PONG_DESCRIPTION) {
    priority = 3;
    stack_depth = 4096;
    canMinimize = false;
}

 
void App_Pong::onOpen() {
    bot_score=0;
    player_score=0;
    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
    batch.fillScreen(BACKGROUND_COLOR);
    print_field_lines(&batch, GAME_COLOR);
    print_score(&batch, GAME_COLOR);
    graphics.endBatch(&batch);
    setup_game();

}

float lerp(float a, float b, float x)
{ 
  return a + x * (b - a);
}

void App_Pong::onTick() {
    tick++;

    if(end==1){
        player_score=1;
        setup_game();
    }

    nbotx = ((int) (sin(0.05 * tick) * (DEFAULT_VIEWPORT.width / 2 - BAR_LENGTH))) + DEFAULT_VIEWPORT.width / 2;

    // Draw top bar
    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
    if (nbotx != botx) {
        print_top_bar(&batch, botx, nbotx);
        botx = nbotx;
    }
  
    TouchData data = touch.get();

    if (data.pressed) {
        int nplayerx = constrain(data.x, BAR_HALF_LENGTH + 5, DEFAULT_VIEWPORT.width - BAR_HALF_LENGTH - 5);

        if (playerx != nplayerx) {
            tplayerx = nplayerx;
        }
    }

    // Draw bot bar
    int iiplayerx = ceil(lerp(playerx, tplayerx, 0.3));
    print_bottom_bar(&batch, playerx, iiplayerx);
    playerx = iiplayerx;
    int cxa=cx;
    int cya=cy;

    update_game(cx,cy,r,tplayerx,nbotx,vx,vy);
    print_ball(&batch,cx,cy,cxa,cya,GAME_COLOR);

    graphics.endBatch(&batch);

    vAppDelay(10 / portTICK_PERIOD_MS);
}

void App_Pong::onClose() {
}
