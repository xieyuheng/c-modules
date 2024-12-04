#include "index.h"

struct example_button_t {
    canvas_t *canvas;
    uint8_t *button_up_chr;
    uint8_t *button_down_chr;
    bool is_pressed;
};

example_button_t *
example_button_new(void) {
    example_button_t *self = allocate(sizeof(example_button_t));

    char *base = dirname(string_dup(__FILE__));
    const char *file_name = string_append(base, "/button10x10.chr");
    file_t *file = fopen(file_name, "rb");
    uint8_t *bytes = file_read_bytes(file);
    self->button_up_chr = chr_subimage(bytes, 0x10, 0, 0, 3, 3);
    self->button_down_chr = chr_subimage(bytes, 0x10, 3, 0, 3, 3);
    free(bytes);

    self->canvas = canvas_new(9 * TILE, 9 * TILE, 0x10);
    self->canvas->title = "example button";
    self->canvas->state = self;

    self->is_pressed = false;

    return self;
}

void
example_button_destroy(example_button_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        example_button_t *self = *self_pointer;
        canvas_destroy(&self->canvas);
        free(self->button_up_chr);
        free(self->button_down_chr);
        free(self);
        *self_pointer = NULL;
    }
}

static void
on_click_button(
    example_button_t *self,
    canvas_t *canvas,
    size_t x,
    size_t y,
    uint8_t button,
    bool is_release
) {
    (void) canvas;
    (void) self;

    (void) x;
    (void) y;

    if (button == 1) {
        if (is_release) {
            self->is_pressed = false;
            printf("!");
        } else {
            self->is_pressed = true;
        }
    }
}

static void
render_button(example_button_t *self, canvas_t *canvas) {
    size_t x = 3 * TILE;
    size_t y = 3 * TILE;
    size_t width = 3 * TILE;
    size_t height = 3 * TILE;

    if (self->is_pressed) {
        canvas_draw_chr_image(canvas, x, y, self->button_down_chr, 3, 3, 1);
        canvas_add_clickable_area(
            canvas, x, y, width, height,
            (on_click_t *) on_click_button);
    } else {
        canvas_draw_chr_image(canvas, x, y, self->button_up_chr, 3, 3, 1);
        canvas_add_clickable_area(
            canvas, x, y, width, height,
            (on_click_t *) on_click_button);
    }
}

static void
on_frame(
    example_button_t *self,
    canvas_t *canvas,
    uint64_t passed
) {
    (void) passed;

    canvas->window->background_pixel = canvas->palette[BG_COLOR];
    canvas_fill_bottom_right(canvas, 0, 0, BG_COLOR);
    canvas_clear_clickable_area(canvas);
    render_button(self, canvas);
}

void
example_button_start(void) {
    example_button_t *self = example_button_new();
    self->canvas->on_frame = (on_frame_t *) on_frame;
    canvas_open(self->canvas);
    example_button_destroy(&self);
}
