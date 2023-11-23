#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <stdlib.h>

#define BARCODE_STARTING_POS 16
#define BARCODE_HEIGHT 50
#define BARCODE_Y_START 3
#define BARCODE_TEXT_OFFSET 9

typedef enum {
    EventTypeTick,
    EventTypeKey,
} EventType;

typedef struct {
    EventType type;
    InputEvent input;
} PluginEvent;

typedef struct {
    int barcodeNumeral[12]; //The current barcode number
    int editingIndex; //The index of the editing symbol
    int menuIndex; //The index of the menu cursor
    int modeIndex; //Set to 0 for view, 1 for edit, 2 for menu
    bool doParityCalculation; //Should do parity check?
} PluginState;

void number_0(
        bool rightHand,
    int startingPosition) { //UPC Code for #0 on left is OOOIIOI
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_str(
        canvas, startingPosition, BARCODE_Y_START + BARCODE_HEIGHT + BARCODE_TEXT_OFFSET, "0");
    if(rightHand) {
        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_set_color(canvas, ColorWhite);
    }
    canvas_draw_box(canvas, startingPosition, BARCODE_Y_START, 3, BARCODE_HEIGHT); //OOO
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 3, BARCODE_Y_START, 2, BARCODE_HEIGHT); //II
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 5, BARCODE_Y_START, 1, BARCODE_HEIGHT); //O
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 6, BARCODE_Y_START, 1, BARCODE_HEIGHT); //I
}
void number_1(
        bool rightHand,
    int startingPosition) { //UPC Code for #1 on left is OOIIOOI
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_str(
        canvas, startingPosition, BARCODE_Y_START + BARCODE_HEIGHT + BARCODE_TEXT_OFFSET, "1");

    if(rightHand) {
        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_set_color(canvas, ColorWhite);
    }
    canvas_draw_box(canvas, startingPosition, BARCODE_Y_START, 2, BARCODE_HEIGHT); //OO
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 2, BARCODE_Y_START, 2, BARCODE_HEIGHT); //II
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 4, BARCODE_Y_START, 2, BARCODE_HEIGHT); //OO
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 6, BARCODE_Y_START, 1, BARCODE_HEIGHT); //I
}
void number_2(
        bool rightHand,
    int startingPosition) { //UPC Code for #2 on left is OOIOOII
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_str(
        canvas, startingPosition, BARCODE_Y_START + BARCODE_HEIGHT + BARCODE_TEXT_OFFSET, "2");
    if(rightHand) {
        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_set_color(canvas, ColorWhite);
    }
    canvas_draw_box(canvas, startingPosition, BARCODE_Y_START, 2, BARCODE_HEIGHT); //OO
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 2, BARCODE_Y_START, 1, BARCODE_HEIGHT); //I
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 3, BARCODE_Y_START, 2, BARCODE_HEIGHT); //OO
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 5, BARCODE_Y_START, 2, BARCODE_HEIGHT); //II
}
void number_3(
        bool rightHand,
    int startingPosition) { //UPC Code for #3 on left is OIIIIOI
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_str(
        canvas, startingPosition, BARCODE_Y_START + BARCODE_HEIGHT + BARCODE_TEXT_OFFSET, "3");
    if(rightHand) {
        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_set_color(canvas, ColorWhite);
    }
    canvas_draw_box(canvas, startingPosition, BARCODE_Y_START, 1, BARCODE_HEIGHT); //O
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 1, BARCODE_Y_START, 4, BARCODE_HEIGHT); //IIII
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 5, BARCODE_Y_START, 1, BARCODE_HEIGHT); //O
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 6, BARCODE_Y_START, 1, BARCODE_HEIGHT); //I
}
void number_4(
        bool rightHand,
    int startingPosition) { //UPC Code for #4 on left is OIOOOII
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_str(
        canvas, startingPosition, BARCODE_Y_START + BARCODE_HEIGHT + BARCODE_TEXT_OFFSET, "4");
    if(rightHand) {
        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_set_color(canvas, ColorWhite);
    }
    canvas_draw_box(canvas, startingPosition, BARCODE_Y_START, 1, BARCODE_HEIGHT); //O
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 1, BARCODE_Y_START, 1, BARCODE_HEIGHT); //I
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 2, BARCODE_Y_START, 3, BARCODE_HEIGHT); //OOO
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 5, BARCODE_Y_START, 2, BARCODE_HEIGHT); //II
}
void number_5(
        bool rightHand,
    int startingPosition) { //UPC Code for #5 on left is OIIOOOI
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_str(
        canvas, startingPosition, BARCODE_Y_START + BARCODE_HEIGHT + BARCODE_TEXT_OFFSET, "5");
    if(rightHand) {
        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_set_color(canvas, ColorWhite);
    }
    canvas_draw_box(canvas, startingPosition, BARCODE_Y_START, 1, BARCODE_HEIGHT); //O
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 1, BARCODE_Y_START, 2, BARCODE_HEIGHT); //II
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 3, BARCODE_Y_START, 3, BARCODE_HEIGHT); //OOO
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 6, BARCODE_Y_START, 1, BARCODE_HEIGHT); //I
}
void number_6(
        bool rightHand,
    int startingPosition) { //UPC Code for #6 on left is OIOIIII
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_str(
        canvas, startingPosition, BARCODE_Y_START + BARCODE_HEIGHT + BARCODE_TEXT_OFFSET, "6");
    if(rightHand) {
        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_set_color(canvas, ColorWhite);
    }
    canvas_draw_box(canvas, startingPosition, BARCODE_Y_START, 1, BARCODE_HEIGHT); //O
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 1, BARCODE_Y_START, 1, BARCODE_HEIGHT); //I
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 2, BARCODE_Y_START, 1, BARCODE_HEIGHT); //O
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 3, BARCODE_Y_START, 4, BARCODE_HEIGHT); //IIII
}
void number_7(
        bool rightHand,
    int startingPosition) { //UPC Code for #7 on left is OIIIOII
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_str(
        canvas, startingPosition, BARCODE_Y_START + BARCODE_HEIGHT + BARCODE_TEXT_OFFSET, "7");
    if(rightHand) {
        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_set_color(canvas, ColorWhite);
    }
    canvas_draw_box(canvas, startingPosition, BARCODE_Y_START, 1, BARCODE_HEIGHT); //O
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 1, BARCODE_Y_START, 3, BARCODE_HEIGHT); //III
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 4, BARCODE_Y_START, 1, BARCODE_HEIGHT); //O
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 5, BARCODE_Y_START, 2, BARCODE_HEIGHT); //II
}
void number_8(
        bool rightHand,
    int startingPosition) { //UPC Code for #8 on left is OIIOIII
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_str(
        canvas, startingPosition, BARCODE_Y_START + BARCODE_HEIGHT + BARCODE_TEXT_OFFSET, "8");
    if(rightHand) {
        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_set_color(canvas, ColorWhite);
    }
    canvas_draw_box(canvas, startingPosition, BARCODE_Y_START, 1, BARCODE_HEIGHT); //O
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 1, BARCODE_Y_START, 2, BARCODE_HEIGHT); //II
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 3, BARCODE_Y_START, 1, BARCODE_HEIGHT); //O
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 4, BARCODE_Y_START, 3, BARCODE_HEIGHT); //III
}
void number_9(
        bool rightHand,
    int startingPosition) { //UPC Code for #9 on left is OOOIOII
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_str(
        canvas, startingPosition, BARCODE_Y_START + BARCODE_HEIGHT + BARCODE_TEXT_OFFSET, "9");
    if(rightHand) {
        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_set_color(canvas, ColorWhite);
    }
    canvas_draw_box(canvas, startingPosition, BARCODE_Y_START, 3, BARCODE_HEIGHT); //OOO
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 3, BARCODE_Y_START, 1, BARCODE_HEIGHT); //I
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 4, BARCODE_Y_START, 1, BARCODE_HEIGHT); //O
    canvas_invert_color(canvas);
    canvas_draw_box(canvas, startingPosition + 5, BARCODE_Y_START, 2, BARCODE_HEIGHT); //II
}