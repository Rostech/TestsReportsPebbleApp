#include <pebble.h>
#include "main.h"

static Window *s_main_window;

static TextLayer *s_time_layer;
static TextLayer *s_passed_tests_info_layer;
static TextLayer *s_failed_tests_inlfo_layer;
static TextLayer *s_last_run_date_info_layer;
static TextLayer *s_last_run_time_info_layer;

static BitmapLayer *s_checkbox_layer;
static GBitmap *s_checkbox_bitmap;
static BitmapLayer *s_cancel_layer;
static GBitmap *s_cancel_bitmap;
static BitmapLayer *s_calendar_layer;
static GBitmap *s_calendar_bitmap;

static void configure_time_layer(Layer *window_layer, GRect bounds) {
  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(16, 10), bounds.size.w, 50));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text_color(s_time_layer, GColorWhite);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void configure_passed_tests_info(Layer *window_layer, GRect bounds) {
  // Create the TextLayer with specific bounds
  s_passed_tests_info_layer = text_layer_create(
      GRect(bounds.size.w / 2, PBL_IF_ROUND_ELSE(48, 42), bounds.size.w / 2, 50));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_passed_tests_info_layer, GColorClear);
  text_layer_set_text(s_passed_tests_info_layer, "...");
  text_layer_set_font(s_passed_tests_info_layer, fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS));
  text_layer_set_text_alignment(s_passed_tests_info_layer, GTextAlignmentLeft);
  text_layer_set_text_color(s_passed_tests_info_layer, GColorWhite);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_passed_tests_info_layer));  
}

static void configure_failed_tests_info(Layer *window_layer, GRect bounds) {
  // Create the TextLayer with specific bounds
  s_failed_tests_inlfo_layer = text_layer_create(
      GRect(bounds.size.w / 2, PBL_IF_ROUND_ELSE(88, 82), bounds.size.w / 2, 50));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_failed_tests_inlfo_layer, GColorClear);
  text_layer_set_text(s_failed_tests_inlfo_layer, "...");
  text_layer_set_font(s_failed_tests_inlfo_layer, fonts_get_system_font(FONT_KEY_LECO_32_BOLD_NUMBERS));
  text_layer_set_text_alignment(s_failed_tests_inlfo_layer, GTextAlignmentLeft);
  text_layer_set_text_color(s_failed_tests_inlfo_layer, GColorWhite);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_failed_tests_inlfo_layer));  
}

static void configure_last_run_date_info(Layer *window_layer, GRect bounds) {
  // Create the TextLayer with specific bounds
  s_last_run_date_info_layer = text_layer_create(
      GRect(bounds.size.w / 2, PBL_IF_ROUND_ELSE(128, 122), bounds.size.w / 2, 50));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_last_run_date_info_layer, GColorClear);
  text_layer_set_text(s_last_run_date_info_layer, "...");
  text_layer_set_font(s_last_run_date_info_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_last_run_date_info_layer, GTextAlignmentLeft);
  text_layer_set_text_color(s_last_run_date_info_layer, GColorWhite);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_last_run_date_info_layer));  
}

static void configure_last_run_time_info(Layer *window_layer, GRect bounds) {
  // Create the TextLayer with specific bounds
  s_last_run_time_info_layer = text_layer_create(
      GRect(bounds.size.w / 2, PBL_IF_ROUND_ELSE(138, 138), bounds.size.w / 2, 50));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_last_run_time_info_layer, GColorClear);
  text_layer_set_text(s_last_run_time_info_layer, "...");
  text_layer_set_font(s_last_run_time_info_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_last_run_time_info_layer, GTextAlignmentLeft);
  text_layer_set_text_color(s_last_run_time_info_layer, GColorWhite);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_last_run_time_info_layer));  
}

static void configure_passed_tests_bitmap(Layer *window_layer, GRect bounds) {  
  // Create GBitmap
  s_checkbox_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHECK);

  // Create BitmapLayer to display the GBitmap
  s_checkbox_layer = bitmap_layer_create(
      GRect(20, PBL_IF_ROUND_ELSE(44, 38), bounds.size.w / 2, 50));

  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_checkbox_layer, s_checkbox_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_checkbox_layer));
  bitmap_layer_set_alignment(s_checkbox_layer, GAlignLeft);
}

static void configure_failed_tests_bitmap(Layer *window_layer, GRect bounds) {  
  // Create GBitmap
  s_cancel_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CANCEL);

  // Create BitmapLayer to display the GBitmap
  s_cancel_layer = bitmap_layer_create(
      GRect(2, PBL_IF_ROUND_ELSE(84, 78), bounds.size.w / 2, 50));

  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_cancel_layer, s_cancel_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_cancel_layer));
  bitmap_layer_set_alignment(s_checkbox_layer, GAlignLeft);
}

static void configure_last_run_date_bitmap(Layer *window_layer, GRect bounds) {  
  // Create GBitmap
  s_calendar_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CALENDAR);

  // Create BitmapLayer to display the GBitmap
  s_calendar_layer = bitmap_layer_create(
      GRect(15, PBL_IF_ROUND_ELSE(124, 118), bounds.size.w / 2, 50));

  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_calendar_layer, s_calendar_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_calendar_layer));
  bitmap_layer_set_alignment(s_calendar_layer, GAlignLeft);
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  configure_passed_tests_bitmap(window_layer, bounds);
  configure_failed_tests_bitmap(window_layer, bounds);
  configure_last_run_date_bitmap(window_layer, bounds);
  configure_time_layer(window_layer, bounds);
  configure_passed_tests_info(window_layer, bounds);
  configure_failed_tests_info(window_layer, bounds);
  configure_last_run_date_info(window_layer, bounds);
  configure_last_run_time_info(window_layer, bounds);
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_passed_tests_info_layer);
  text_layer_destroy(s_failed_tests_inlfo_layer);
  text_layer_destroy(s_last_run_date_info_layer);
  text_layer_destroy(s_last_run_time_info_layer);
  
  // Destroy GBitmap
  gbitmap_destroy(s_checkbox_bitmap);
  gbitmap_destroy(s_cancel_bitmap);
  gbitmap_destroy(s_calendar_bitmap);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_checkbox_layer);
  bitmap_layer_destroy(s_cancel_layer);
  bitmap_layer_destroy(s_calendar_layer);
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char passed_buffer[8];
  static char failed_buffer[8];
  static char lastExecuted_date_buffer[16];
  static char lastExecuted_time_buffer[16];
  
  static char passed_layer_buffer[8];
  static char failed_layer_buffer[8];
  static char date_layer_buffer[16];
  static char time_layer_buffer[16];
  
  // Read tuples for data
  Tuple *passed_tuple = dict_find(iterator, MESSAGE_KEY_PASSED);
  Tuple *failed_tuple = dict_find(iterator, MESSAGE_KEY_FAILED);
  Tuple *lastExecuted_date_tuple = dict_find(iterator, MESSAGE_KEY_LASTEXECUTEDDATE);
  Tuple *lastExecuted_time_tuple = dict_find(iterator, MESSAGE_KEY_LASTEXECUTEDTIME);
  
  // If all data is available, use it
  if(passed_tuple && failed_tuple && lastExecuted_date_tuple && lastExecuted_time_tuple) {
    snprintf(passed_buffer, sizeof(passed_buffer), "%d", (int)passed_tuple->value->int32);
    snprintf(failed_buffer, sizeof(failed_buffer), "%d", (int)failed_tuple->value->int32);
    snprintf(lastExecuted_date_buffer, sizeof(lastExecuted_date_buffer), "%s", lastExecuted_date_tuple->value->cstring);
    snprintf(lastExecuted_time_buffer, sizeof(lastExecuted_time_buffer), "%s", lastExecuted_time_tuple->value->cstring);
  }
  
  // Assemble full string and display
  snprintf(passed_layer_buffer, sizeof(passed_layer_buffer), "%s", passed_buffer);
  text_layer_set_text(s_passed_tests_info_layer, passed_layer_buffer);
  
  snprintf(failed_layer_buffer, sizeof(failed_layer_buffer), "%s", failed_buffer);
  text_layer_set_text(s_failed_tests_inlfo_layer, failed_layer_buffer);
  
  snprintf(date_layer_buffer, sizeof(date_layer_buffer), "%s", lastExecuted_date_buffer);
  text_layer_set_text(s_last_run_date_info_layer, date_layer_buffer);
  
  snprintf(time_layer_buffer, sizeof(time_layer_buffer), "%s", lastExecuted_time_buffer);
  text_layer_set_text(s_last_run_time_info_layer, time_layer_buffer);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped! :(");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed! :(");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success! :)");
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_set_background_color(s_main_window, GColorBlack);

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  // Make sure the time is displayed from the start
  update_time();
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  app_message_register_inbox_received(inbox_received_callback);
  
  const int inbox_size = 128;
  const int outbox_size = 128;
  app_message_open(inbox_size, outbox_size);
  
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
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