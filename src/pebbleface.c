#include <pebble.h>

static Window *s_window;
static TextLayer *s_text_layer;
static AppTimer *s_click_timer;
static bool s_click_timer_running;
 
static void click_config_provider(void *context);
static void select_down_handler(ClickRecognizerRef recognizer, void *context);
static void select_up_handler(ClickRecognizerRef recognizer, void *context);
static void long_press_handler(void *data);

static void main_window_load();
static void main_window_unload();

// ------

static void init() {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_set_click_config_provider(s_window, click_config_provider);
  window_stack_push(s_window, true);
}

static void deinit() {
  window_destroy(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

// ------

static void main_window_load() {
  Layer *root_layer = window_get_root_layer(s_window);
  s_text_layer = text_layer_create(layer_get_frame(root_layer));
  text_layer_set_text(s_text_layer, "Testing...");
  layer_add_child(root_layer, text_layer_get_layer(s_text_layer));
}

static void main_window_unload() {
  text_layer_destroy(s_text_layer);
}

// ------

static void click_config_provider(void *context) {
  window_raw_click_subscribe(BUTTON_ID_SELECT, select_down_handler, select_up_handler, NULL);
}
 
static void select_down_handler(ClickRecognizerRef recognizer, void *context) {
  s_click_timer = app_timer_register(1000, long_press_handler, NULL);
  s_click_timer_running = true;
}
 
static void select_up_handler(ClickRecognizerRef recognizer, void *context) {
  // if the button is released after the timeout, the long press 
  // has happened - do nothing
  if (!s_click_timer_running) return;
 
  app_timer_cancel(s_click_timer);
  s_click_timer_running = false;
 
  text_layer_set_text(s_text_layer, "Short click");
}
 
static void long_press_handler(void *data) {
  s_click_timer_running = false;
 
  text_layer_set_text(s_text_layer, "Loooong click");
}
