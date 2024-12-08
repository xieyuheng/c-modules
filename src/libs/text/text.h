#pragma once

text_t *text_new(size_t length);
void text_destroy(text_t **self_pointer);

text_t *text_from_string(const char *string);

size_t text_length(text_t *self);
