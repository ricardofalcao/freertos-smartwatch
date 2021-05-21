#include "app/pong.h"

#include "graphics.h"
#include "touch.h"
#include "lang/lang.h"

#define TIMESTEP_MS             10

#define PONG_DEFAULT_BALL_VELOCITY    5

#define BACKGROUND_COLOR        TFT_BLACK
#define GAME_COLOR              TFT_WHITE

#define MARGIN_Y                35
#define MARGIN_X                35

#define BAR_LENGTH              50
#define BAR_HALF_LENGTH         (BAR_LENGTH / 2)

#define BAR_HEIGHT              10
#define BAR_HALF_HEIGHT         (BAR_HEIGHT / 2)

#define SPACE_BETWEEN_BARS      6
#define DASHED_BARS_LENGTH      10

//

#define STATE_PLAY              0
#define STATE_WIN               1

//

#define BALL_RADIUS             6

#define MAX_BOUNCE_ANGLE        0.8726646259971648f

bool _circle_intersects_rect(float cx, float cy, float radius, float rx, float ry, float rw, float rh) {
    float distX = abs(cx - rx-rw/2);
    float distY = abs(cy - ry-rh/2);
    if (distX > (rw/2 + radius)) { return false; }
    if (distY > (rh/2 + radius)) { return false; }
    if (distX <= (rw/2)) { return true; } 
    if (distY <= (rh/2)) { return true; }
    float dx=distX-rw/2;
    float dy=distY-rh/2;
    return (dx*dx+dy*dy<=(radius*radius));
}


float _lerp(float a, float b, float x)
{ 
  return a + x * (b - a);
}

void App_Pong::print_top_bar(GBatch_t * batch, int previous_x, int new_x) {
    if (new_x > previous_x) {
        batch->fillRectangle(previous_x - BAR_HALF_LENGTH, MARGIN_Y, new_x - previous_x, BAR_HEIGHT, BACKGROUND_COLOR);
    } else {
        batch->fillRectangle(new_x + BAR_HALF_LENGTH, MARGIN_Y, previous_x - new_x, BAR_HEIGHT, BACKGROUND_COLOR);
    }

    //place_x means the center of the bar 
    batch->fillRectangle(new_x - BAR_HALF_LENGTH, MARGIN_Y, BAR_LENGTH, BAR_HEIGHT, GAME_COLOR);
}

void App_Pong::print_bottom_bar(GBatch_t * batch, int previous_x, int new_x) {
    if (new_x > previous_x) {
        batch->fillRectangle(previous_x - BAR_HALF_LENGTH, batch->viewHeight() - MARGIN_Y - BAR_HEIGHT, new_x - previous_x, BAR_HEIGHT, BACKGROUND_COLOR);
    } else {
        batch->fillRectangle(new_x + BAR_HALF_LENGTH, batch->viewHeight() - MARGIN_Y - BAR_HEIGHT, previous_x - new_x, BAR_HEIGHT, BACKGROUND_COLOR);
    }
    //place_x means the center of the bar 
    batch->fillRectangle(new_x - BAR_HALF_LENGTH, batch->viewHeight() - MARGIN_Y - BAR_HEIGHT, BAR_LENGTH, BAR_HEIGHT, GAME_COLOR);
}

void App_Pong::print_middle_lines(GBatch_t * batch, uint32_t color) {
    int numberOfLines = (batch->viewWidth() - DASHED_BARS_LENGTH) / (DASHED_BARS_LENGTH + SPACE_BETWEEN_BARS) + 1;
    int offsetX = ((batch->viewWidth() - DASHED_BARS_LENGTH) % (DASHED_BARS_LENGTH + SPACE_BETWEEN_BARS)) / 2;

    for(int i = 0; i < numberOfLines; i++) {
        int x = offsetX + i * (DASHED_BARS_LENGTH+SPACE_BETWEEN_BARS);

        batch->fillRectangle(x, batch->viewHeight() / 2 - 1, DASHED_BARS_LENGTH, 3, color);
    }

}

void App_Pong::print_field_lines(GBatch_t * batch, uint32_t color) {
    batch->drawLine(1, 1, 1, batch->viewHeight(), color, 8);
    batch->drawLine(batch->viewWidth(), 1, batch->viewWidth(), batch->viewHeight(), color, 8);

}

void App_Pong::print_score(GBatch_t * batch, uint32_t color) {

    sprintf(_bot_score, " %d ", bot_score);
    batch->drawFilledString(batch->viewWidth() - MARGIN_X, batch->viewHeight()/2 - MARGIN_Y, _bot_score, color, BACKGROUND_COLOR, 4, MC_DATUM);
    
    sprintf(_player_score, " %d ", player_score);
    batch->drawFilledString(batch->viewWidth() - MARGIN_X, batch->viewHeight()/2 + MARGIN_Y, _player_score, color, BACKGROUND_COLOR, 4, MC_DATUM);
}

void App_Pong::print_ball(GBatch_t * batch, int x_center, int y_center, int previous_x_center, int previous_y_center) {
    batch->fillCircle(previous_x_center, previous_y_center, BALL_RADIUS, BACKGROUND_COLOR);
    batch->fillCircle(x_center, y_center, BALL_RADIUS, GAME_COLOR);
}


App_Pong::App_Pong() : App(MSG_PONG_NAME, MSG_PONG_DESCRIPTION) {
    priority = 3;
    stack_depth = 4096;
    canMinimize = false;
}

void App_Pong::onOpen() {
    setup_game();
}

 
void App_Pong::setup_game() {
    bot_score = 0;
    player_score = 0;

    playerx = DEFAULT_VIEWPORT.width / 2;
    tplayerx = playerx;

    botx = DEFAULT_VIEWPORT.width / 2;

    ballx = DEFAULT_VIEWPORT.width / 2;
    bally = DEFAULT_VIEWPORT.height / 2;

    float angle = random(0, 360) * PI / 180;
    ballvx = PONG_DEFAULT_BALL_VELOCITY * cos(angle);
    ballvy = PONG_DEFAULT_BALL_VELOCITY * sin(angle);

    state = STATE_PLAY;

    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
    batch.fillScreen(BACKGROUND_COLOR);
    print_middle_lines(&batch, GAME_COLOR);
    print_field_lines(&batch, GAME_COLOR);
    print_score(&batch, GAME_COLOR);
    print_top_bar(&batch, botx, botx);
    print_bottom_bar(&batch, playerx, playerx);
    graphics.endBatch(&batch);

    deadTime = 1000 / TIMESTEP_MS;
}

 
void App_Pong::reset_game() {
    int oballx = ballx;
    int obally = bally;

    ballx = DEFAULT_VIEWPORT.width / 2;
    bally = DEFAULT_VIEWPORT.height / 2;

    float angle = random(0, 360) * PI / 180;
    ballvx = PONG_DEFAULT_BALL_VELOCITY * cos(angle);
    ballvy = PONG_DEFAULT_BALL_VELOCITY * sin(angle);

    state = STATE_PLAY;

    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
    print_middle_lines(&batch, GAME_COLOR);
    print_field_lines(&batch, GAME_COLOR);
    print_score(&batch, GAME_COLOR);
    print_ball(&batch, ballx, bally, oballx, obally);
    print_top_bar(&batch, botx, botx);
    print_bottom_bar(&batch, playerx, playerx);
    graphics.endBatch(&batch);

    deadTime = 1000 / TIMESTEP_MS;
}

void App_Pong::update_game(){
    int nballx= ballx + ballvx;
    int nbally = bally + ballvy;

    if (_circle_intersects_rect(nballx, nbally, BALL_RADIUS, -5, -5, 6, DEFAULT_VIEWPORT.height) || _circle_intersects_rect(nballx, nbally, BALL_RADIUS, DEFAULT_VIEWPORT.width - 2, 0, 6, DEFAULT_VIEWPORT.height)) {
        ballvx = -ballvx;
    }

    if (_circle_intersects_rect(nballx, nbally, BALL_RADIUS, playerx - BAR_HALF_LENGTH, DEFAULT_VIEWPORT.height - MARGIN_Y - BAR_HEIGHT, BAR_LENGTH, BAR_HEIGHT)) {
        float relativeIntersectX = playerx - ballx;
        float normalizedRelativeIntersectionX = relativeIntersectX / (BAR_LENGTH / 2);
        float bounceAngle = normalizedRelativeIntersectionX * MAX_BOUNCE_ANGLE;

        ballvy = PONG_DEFAULT_BALL_VELOCITY*-cos(bounceAngle);
        ballvx = PONG_DEFAULT_BALL_VELOCITY*-sin(bounceAngle);

    } else if (_circle_intersects_rect(nballx, nbally, BALL_RADIUS, botx - BAR_HALF_LENGTH, MARGIN_Y, BAR_LENGTH, BAR_HEIGHT))  {
        float relativeIntersectX = botx - ballx;
        float normalizedRelativeIntersectionX = relativeIntersectX / (BAR_LENGTH / 2);
        float bounceAngle = normalizedRelativeIntersectionX * MAX_BOUNCE_ANGLE;

        ballvy = PONG_DEFAULT_BALL_VELOCITY*cos(bounceAngle);
        ballvx = PONG_DEFAULT_BALL_VELOCITY*-sin(bounceAngle);

    }

    ballx += ballvx;
    bally += ballvy;

    if (bally < MARGIN_Y) {
        state = STATE_WIN;

        player_score++;
    } else if (bally > (DEFAULT_VIEWPORT.height - MARGIN_Y)) {
        state = STATE_WIN;

        bot_score++;
    }
}

void App_Pong::onTick() {
    switch (state) {
        case STATE_PLAY: {
            int cballx = ballx;
            int cbally = bally;

            if (deadTime == 0) {
                update_game();
            } else {
                deadTime--;
            }

            TouchData data = touch.get();

            if (data.pressed) {
                int nplayerx = constrain(data.x, BAR_HALF_LENGTH + 5, DEFAULT_VIEWPORT.width - BAR_HALF_LENGTH - 5);

                if (playerx != nplayerx) {
                    tplayerx = nplayerx;
                }
            }

            // Draw bot bar
            GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);

            if (cballx != ballx || cbally != bally) {
                print_ball(&batch, ballx, bally, cballx, cbally);
            }

            float iiplayerx = ceil(_lerp(playerx, tplayerx, 0.3));
            
            if (playerx != iiplayerx || cbally > DEFAULT_VIEWPORT.height - MARGIN_Y - BAR_HEIGHT - BALL_RADIUS) {
                print_bottom_bar(&batch, playerx, iiplayerx);
            }

            playerx = iiplayerx;

            float tempbotx = botx;
            
            if (tick % (bot_tick_ms / TIMESTEP_MS) == 0) {
                if (tempbotx < ballx - 3) {
                    tempbotx = min(tempbotx + bot_max_speed, ballx);
                } else if (tempbotx > ballx + 3) {
                    tempbotx = max(tempbotx - bot_max_speed, ballx);
                }
            }

            float nbotx = constrain(tempbotx, BAR_HALF_LENGTH + 5, DEFAULT_VIEWPORT.width - BAR_HALF_LENGTH - 5);

            // Draw top bar
            if (nbotx != botx || cbally < MARGIN_Y + BAR_HEIGHT + BALL_RADIUS) {
                print_top_bar(&batch, botx, nbotx);
                botx = nbotx;
            }

            graphics.endBatch(&batch);

            
            if (tick % (100 / TIMESTEP_MS) == 0) {
                batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                print_score(&batch, GAME_COLOR);
                print_middle_lines(&batch, GAME_COLOR);
                graphics.endBatch(&batch);
            Serial.print(1);
            }

            tick++;
            vAppDelay(TIMESTEP_MS / portTICK_PERIOD_MS);
            break;
        }

        case STATE_WIN: {
            reset_game();
            break;
        }
    }
}

void App_Pong::onClose() {
}
