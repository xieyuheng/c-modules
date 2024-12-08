#include "index.h"

struct text_t {
    size_t length;
    code_point_t *code_points;
};

text_t *
text_new(size_t length) {
    text_t *self = new(text_t);
    self->length = length;
    self->code_points = allocate_many(length, sizeof(text_t));
    return self;
}

void
text_destroy(text_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        text_t *self = *self_pointer;
        free(self->code_points);
        free(self);
        *self_pointer = NULL;
    }
}

text_t *
text_from_string(const char *string) {
    size_t length = utf8_string_length(string);
    text_t *text = text_new(length);

    utf8_iter_t *iter = utf8_iter_new(string);
    code_point_t code_point = utf8_iter_start(iter);
    size_t index = 0;
    while (code_point) {
        text->code_points[index] = code_point;
        code_point = utf8_iter_next(iter);
        index++;
    }

    return text;
}

size_t
text_length(text_t *self) {
    return self->length;
}

code_point_t
text_get(text_t *self, size_t index) {
    return self->code_points[index];
}

bool
text_equal(text_t *left, text_t *right) {
    if (left->length != right->length)
        return false;

    return memcmp(
        left->code_points,
        right->code_points,
        left->length * sizeof(code_point_t)) == 0;
}

text_t *
text_dup(text_t *self) {
    text_t *text = text_new(self->length);

    memcpy(
        text->code_points,
        self->code_points,
        self->length * sizeof(code_point_t));

    return text;
}

text_t *
text_append(text_t *left, text_t *right) {
    text_t *text = text_new(left->length + right->length);

    memcpy(
        text->code_points,
        left->code_points,
        left->length * sizeof(code_point_t));

    memcpy(
        text->code_points + left->length,
        right->code_points,
        right->length * sizeof(code_point_t));

    return text;
}

text_t *
text_slice(text_t *self, size_t start, size_t end) {
    assert(end >= start);
    assert(end <= text_length(self));
    size_t length = end - start;
    text_t *text = text_new(length);

    memcpy(
        text->code_points,
        self->code_points + start,
        length * sizeof(code_point_t));

    return text;
}

char *
text_to_string(text_t *self) {
    char *string
}
