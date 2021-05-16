#include "app/lang.h"

#include "wifi.h"
#include "time.h"

#include "touch.h"
#include "graphics.h"

#include "icons/flags/germany.h"
#include "icons/flags/uk.h"
#include "icons/flags/portugal.h"

#define MARGIN_X    30
#define MARGIN_Y    30

#define SPACING_X   10
#define SPACING_Y   20

App_Lang::App_Lang() : App(MSG_LANG_NAME, MSG_LANG_DESCRIPTION) {
    priority = 3;
    touch_stack_depth = 10240;

    icon_de.setColorDepth(4);
    icon_en.setColorDepth(4);
    icon_pt.setColorDepth(4);

    touch_de.x = MARGIN_X;
    touch_de.y = MARGIN_Y;
    touch_de.width = DEFAULT_VIEWPORT.width - 2*MARGIN_X;
    touch_de.height = 64;

    touch_en.x = MARGIN_X;
    touch_en.y = MARGIN_Y + 64 + SPACING_Y;
    touch_en.width = DEFAULT_VIEWPORT.width - 2*MARGIN_X;
    touch_en.height = 64;

    touch_pt.x = MARGIN_X;
    touch_pt.y = MARGIN_Y + 2*(64 + SPACING_Y);
    touch_pt.width = DEFAULT_VIEWPORT.width - 2*MARGIN_X;
    touch_pt.height = 64;
}

void App_Lang::onOpen() {
    icon_de.createSprite(ICON_FLAG_GERMANY_WIDTH, ICON_FLAG_GERMANY_HEIGHT);
    icon_de.pushImage(0, 0, ICON_FLAG_GERMANY_WIDTH, ICON_FLAG_GERMANY_HEIGHT, (uint16_t *) icon_flag_germany);
    icon_de.createPalette(icon_flag_germany_palette, 16);

    icon_en.createSprite(ICON_FLAG_UK_WIDTH, ICON_FLAG_UK_HEIGHT);
    icon_en.pushImage(0, 0, ICON_FLAG_UK_WIDTH, ICON_FLAG_UK_HEIGHT, (uint16_t *) icon_flag_uk);
    icon_en.createPalette(icon_flag_uk_palette, 16);

    icon_pt.createSprite(ICON_FLAG_PORTUGAL_WIDTH, ICON_FLAG_PORTUGAL_HEIGHT);
    icon_pt.pushImage(0, 0, ICON_FLAG_PORTUGAL_WIDTH, ICON_FLAG_PORTUGAL_HEIGHT, (uint16_t *) icon_flag_portugal);
    icon_pt.createPalette(icon_flag_portugal_palette, 16);

    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);

    batch.fillScreen(TFT_WHITE);
    draw_langs(&batch);

    graphics.endBatch(&batch);
}

void App_Lang::onResume() {
    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);

    batch.fillScreen(TFT_WHITE);
    draw_langs(&batch);
    graphics.endBatch(&batch);
}

void App_Lang::onTouchTick() {
    TouchData data = touch.waitRelease();

    if (touch_de.contains(DEFAULT_VIEWPORT, data)) {
        lang.setLanguage(&LANG_DE);

        GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
        draw_langs(&batch);
        graphics.endBatch(&batch);
        return;
    }

    if (touch_en.contains(DEFAULT_VIEWPORT, data)) {
        lang.setLanguage(&LANG_EN);

        GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
        draw_langs(&batch);
        graphics.endBatch(&batch);
        return;
    }

    if (touch_pt.contains(DEFAULT_VIEWPORT, data)) {
        lang.setLanguage(&LANG_PT);

        GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
        draw_langs(&batch);
        graphics.endBatch(&batch);
        return;
    }
}

void App_Lang::onClose() {
}

void App_Lang::draw_langs(GBatch_t * batch) {
    const Language_t * active = lang.getLanguage();

    int32_t y = MARGIN_Y;
    batch->drawImage(&icon_de, MARGIN_X, y);
    batch->drawString(MARGIN_X + icon_de.width() + SPACING_X, y + icon_de.height() / 2 - 2, "German", active == &LANG_DE ? TFT_DARKGREEN : TFT_BLACK, 3, ML_DATUM);
    y += icon_de.height() + SPACING_Y;

    batch->drawImage(&icon_en, MARGIN_X, y);
    batch->drawString(MARGIN_X + icon_en.width() + SPACING_X, y + icon_en.height() / 2 - 2, "English", active == &LANG_EN ? TFT_DARKGREEN : TFT_BLACK, 3, ML_DATUM);
    y += icon_en.height() + SPACING_Y;

    batch->drawImage(&icon_pt, MARGIN_X, y);
    batch->drawString(MARGIN_X + icon_pt.width() + SPACING_X, y + icon_pt.height() / 2 - 2, "Português", active == &LANG_PT ? TFT_DARKGREEN : TFT_BLACK, 3, ML_DATUM);
}