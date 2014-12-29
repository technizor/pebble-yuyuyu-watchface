#include <pebble.h>

Window *window;
TextLayer *text_layer;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

void handle_init(void) {
  window = window_create();

  // Background Image
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_i_yuyuyu_back);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  // Time
  text_layer = text_layer_create(GRect(0, 0, 144, 20));
  
  // Date
  
  window_stack_push(window, true);
}

void handle_deinit(void) {
  
  text_layer_destroy(text_layer);
  gbitmap_destroy(s_background_bitmap);
  bitmap_layer_destroy(s_background_layer);
  window_destroy(window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
