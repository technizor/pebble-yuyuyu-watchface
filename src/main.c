#include <pebble.h>
#include "png.h"

static uint32_t m01[10]={
RESOURCE_ID_i_karin_m01a,
RESOURCE_ID_i_karin_m01b,
RESOURCE_ID_i_karin_m01c,
RESOURCE_ID_i_karin_m01d,
RESOURCE_ID_i_karin_m01e,
RESOURCE_ID_i_karin_m01f,
RESOURCE_ID_i_karin_m01g,
RESOURCE_ID_i_karin_m01h,
RESOURCE_ID_i_karin_m01i,
RESOURCE_ID_i_karin_m01j
};
static uint32_t m10[6]={
RESOURCE_ID_i_karin_m10a,
RESOURCE_ID_i_karin_m10b,
RESOURCE_ID_i_karin_m10c,
RESOURCE_ID_i_karin_m10d,
RESOURCE_ID_i_karin_m10e,
RESOURCE_ID_i_karin_m10f
};
static uint32_t s01[10]={
RESOURCE_ID_i_karin_s01a,
RESOURCE_ID_i_karin_s01b,
RESOURCE_ID_i_karin_s01c,
RESOURCE_ID_i_karin_s01d,
RESOURCE_ID_i_karin_s01e,
RESOURCE_ID_i_karin_s01f,
RESOURCE_ID_i_karin_s01g,
RESOURCE_ID_i_karin_s01h,
RESOURCE_ID_i_karin_s01i,
RESOURCE_ID_i_karin_s01j
};
static uint32_t s10[6]={
RESOURCE_ID_i_karin_s10a,
RESOURCE_ID_i_karin_s10b,
RESOURCE_ID_i_karin_s10c,
RESOURCE_ID_i_karin_s10d,
RESOURCE_ID_i_karin_s10e,
RESOURCE_ID_i_karin_s10f
};
  
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static BitmapLayer *s_character_layer;
static BitmapLayer *s_flower_base_layer;
static GBitmap *s_character_bitmap;
static GBitmap *s_flower_base_bitmap;

static BitmapLayer *s_flower_anim_layer_s01;
static BitmapLayer *s_flower_anim_layer_s10;
static BitmapLayer *s_flower_anim_layer_m01;
static BitmapLayer *s_flower_anim_layer_m10;
/*
static GBitmap *s_flower_anim_bitmap_s01[10];
static GBitmap *s_flower_anim_bitmap_s10[6];
static GBitmap *s_flower_anim_bitmap_m01[10];
static GBitmap *s_flower_anim_bitmap_m10[6];
//*/

static GBitmap *s_flower_anim_bitmap_s01;
static GBitmap *s_flower_anim_bitmap_s10;
static GBitmap *s_flower_anim_bitmap_m01;
static GBitmap *s_flower_anim_bitmap_m10;

static uint lastSec;
static uint lastMin;
/*
void  loadS01();
void  loadS10();
 void loadM01();
 void loadM10();
//*/
static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("00:00:00"), "%H:%M:%S", tick_time);
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00:00"), "%I:%M:%S", tick_time);
  }
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
  
  static char buffer2[] = "Sun Jan 01";
  strftime(buffer2, sizeof("Sun Jan 01"), "%a %b %d", tick_time);
  text_layer_set_text(s_date_layer, buffer2);
  
  // Update flower animations
  uint thisSec = tick_time->tm_sec;
  uint thisMin = tick_time->tm_min;
  /*
  bitmap_layer_set_bitmap(s_flower_anim_layer_s01,s_flower_anim_bitmap_s01[thisSec%10]);
  bitmap_layer_set_bitmap(s_flower_anim_layer_s10,s_flower_anim_bitmap_s10[thisSec/10]);
  bitmap_layer_set_bitmap(s_flower_anim_layer_m01,s_flower_anim_bitmap_m01[thisMin%10]); 
  bitmap_layer_set_bitmap(s_flower_anim_layer_m10,s_flower_anim_bitmap_m10[thisMin/10]);
  //*/
  //*
  if(thisSec%10 != lastSec%10)
    {
  gbitmap_destroy(s_flower_anim_bitmap_s01);
  s_flower_anim_bitmap_s01 = gbitmap_create_with_png_resource(s01[thisSec%10]);
  bitmap_layer_set_bitmap(s_flower_anim_layer_s01, s_flower_anim_bitmap_s01);
  }
  if(thisSec/10 != lastSec/10)
    {
  gbitmap_destroy(s_flower_anim_bitmap_s10);
  s_flower_anim_bitmap_s10 = gbitmap_create_with_png_resource(s10[thisSec/10]);
  bitmap_layer_set_bitmap(s_flower_anim_layer_s10, s_flower_anim_bitmap_s10);
  }
  if(thisMin%10 != lastMin%10)
    {
  gbitmap_destroy(s_flower_anim_bitmap_m01);
  s_flower_anim_bitmap_m01 = gbitmap_create_with_png_resource(m01[thisMin%10]);
  bitmap_layer_set_bitmap(s_flower_anim_layer_m01, s_flower_anim_bitmap_m01);
  }
  if(thisMin/10 != lastMin/10)
    {
  gbitmap_destroy(s_flower_anim_bitmap_m10);
  s_flower_anim_bitmap_m10 = gbitmap_create_with_png_resource(m10[thisMin/10]);
  bitmap_layer_set_bitmap(s_flower_anim_layer_m10, s_flower_anim_bitmap_m10);
  }
  lastSec = thisSec;
  lastMin = thisMin;
  //*/
}

static void main_window_load(Window *window) {
  
  // Background Image
  s_background_bitmap = gbitmap_create_with_png_resource(RESOURCE_ID_i_yuyuyu_back);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  // Character Base Image
  s_character_bitmap = gbitmap_create_with_png_resource(RESOURCE_ID_i_character_karin);
  s_character_layer = bitmap_layer_create(GRect(0,42,60,84));
  bitmap_layer_set_compositing_mode(s_character_layer,GCompOpAnd);
  bitmap_layer_set_bitmap(s_character_layer,s_character_bitmap);
  // Flower Base Image
  s_flower_base_bitmap = gbitmap_create_with_png_resource(RESOURCE_ID_i_flower_base_karin);
  s_flower_base_layer = bitmap_layer_create(GRect(60,42,84,84));
  bitmap_layer_set_bitmap(s_flower_base_layer,s_flower_base_bitmap);
 
  // Create Animation Layers
  //*
  s_flower_anim_layer_s01 = bitmap_layer_create(GRect(60,42,84,84));
  s_flower_anim_layer_s10 = bitmap_layer_create(GRect(60,42,84,84));
  s_flower_anim_layer_m01 = bitmap_layer_create(GRect(60,42,84,84));
  s_flower_anim_layer_m10 = bitmap_layer_create(GRect(60,42,84,84));
  //*/
  /*
  s_flower_anim_layer_s01 = bitmap_layer_create(GRect(0,-6,84,84));
  s_flower_anim_layer_s10 = bitmap_layer_create(GRect(84,-6,84,84));
  s_flower_anim_layer_m01 = bitmap_layer_create(GRect(0,66,84,84));
  s_flower_anim_layer_m10 = bitmap_layer_create(GRect(84,66,84,84));
  //*/
  bitmap_layer_set_compositing_mode(s_flower_anim_layer_s01,GCompOpAnd);
  bitmap_layer_set_compositing_mode(s_flower_anim_layer_s10,GCompOpAnd);
  bitmap_layer_set_compositing_mode(s_flower_anim_layer_m01,GCompOpAnd);
  bitmap_layer_set_compositing_mode(s_flower_anim_layer_m10,GCompOpAnd);
  
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 125, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00:00");
  text_layer_set_font(s_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TERMINUS_NUMBERS_32)));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  // Create date TextLayer
  s_date_layer = text_layer_create(GRect(0, 5, 144, 50));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_text(s_date_layer, "Sun Jan 01");
  text_layer_set_font(s_date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TERMINUS_24)));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  //*
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_character_layer));
  
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_flower_base_layer));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_flower_anim_layer_m10));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_flower_anim_layer_m01));
  layer_add_child(window_get_root_layer(window),bitmap_layer_get_layer(s_flower_anim_layer_s10));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_flower_anim_layer_s01));
  //*/
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer)); 
  //*/
  
  // Make sure the time is displayed from the start
  lastSec=61;
  lastMin=61;
  update_time();
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  
  bitmap_layer_destroy(s_background_layer);
  gbitmap_destroy(s_background_bitmap);
  
  bitmap_layer_destroy(s_character_layer);
  gbitmap_destroy(s_character_bitmap);
  
  bitmap_layer_destroy(s_flower_base_layer);
  gbitmap_destroy(s_flower_base_bitmap);
  
  bitmap_layer_destroy(s_flower_anim_layer_s01);
  bitmap_layer_destroy(s_flower_anim_layer_s10);
  bitmap_layer_destroy(s_flower_anim_layer_m01);
  bitmap_layer_destroy(s_flower_anim_layer_m10);
  
  gbitmap_destroy(s_flower_anim_bitmap_s01);
  gbitmap_destroy(s_flower_anim_bitmap_s10);
  gbitmap_destroy(s_flower_anim_bitmap_m01);
  gbitmap_destroy(s_flower_anim_bitmap_m10);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
  
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

