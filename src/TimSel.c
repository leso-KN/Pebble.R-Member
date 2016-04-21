#include <pebble.h>

static Window *selTimWindow;
static TextLayer *selTim_BG;
static TextLayer *selTim_tx_DP;
static TextLayer *selTim_tx_hour;
static TextLayer *selTim_tx_min;
static GFont fnt_lcd;
static bool atMin;
static time_t selTim_ticks;

static void renderTime()
{
  struct tm *tick_time = localtime(&selTim_ticks);
  static char bufferH[] = "00";
  static char bufferM[] = "00";
  strftime(bufferH, sizeof("00"), "%H", tick_time);
  strftime(bufferM, sizeof("00"), "%M", tick_time);

  text_layer_set_text(selTim_tx_hour, bufferH);
  text_layer_set_text(selTim_tx_DP, ":");
  text_layer_set_text(selTim_tx_min, bufferM);
}

static void main_window_load(Window *window) {
  selTim_BG = text_layer_create(GRect(0,0,144,164));
  text_layer_set_background_color(selTim_BG, GColorBlack);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(selTim_BG));
  
  selTim_tx_hour = text_layer_create(GRect(30, 52, 35, 37));
  selTim_tx_DP = text_layer_create(GRect(65, 52, 14, 37));
  selTim_tx_min = text_layer_create(GRect(79, 52, 35, 37));
    
  text_layer_set_background_color(selTim_tx_hour, GColorWhite);
  text_layer_set_text_color(selTim_tx_hour, GColorBlack);
  text_layer_set_background_color(selTim_tx_DP, GColorBlack);
  text_layer_set_text_color(selTim_tx_DP, GColorClear);
  text_layer_set_background_color(selTim_tx_min, GColorBlack);
  text_layer_set_text_color(selTim_tx_min, GColorClear);
  
  text_layer_set_text(selTim_tx_DP, ":");
  selTim_ticks = time(NULL);
  renderTime();
  
  text_layer_set_text_alignment(selTim_tx_hour, GTextAlignmentCenter);
  text_layer_set_text_alignment(selTim_tx_DP, GTextAlignmentCenter);
  text_layer_set_text_alignment(selTim_tx_min, GTextAlignmentCenter);

  fnt_lcd = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LCD_DISP_NORMAL_28));
  
  text_layer_set_font(selTim_tx_hour, fnt_lcd);
  text_layer_set_font(selTim_tx_DP, fnt_lcd);
  text_layer_set_font(selTim_tx_min, fnt_lcd);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(selTim_tx_hour));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(selTim_tx_DP));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(selTim_tx_min));
}

static void btn_select()
{
  if (atMin)
  {
    window_stack_pop_all(true);
  }
  else
  {
    atMin = true;
    text_layer_set_background_color(selTim_tx_min, GColorWhite);
    text_layer_set_text_color(selTim_tx_min, GColorBlack);
    text_layer_set_background_color(selTim_tx_hour, GColorBlack);
    text_layer_set_text_color(selTim_tx_hour, GColorClear);
  }
}
static void btn_up()
{
  if (atMin)
  {
    selTim_ticks+=60;
  }
  else
  {
    selTim_ticks+=3600;
  }
  renderTime();
}

static void btn_down()
{
  if (atMin)
  {
    selTim_ticks-=60;
  }
  else
  {
    selTim_ticks-=3600;
  }
  renderTime();
}

static void clickConfig(){
  window_single_click_subscribe(BUTTON_ID_SELECT, btn_select);
  window_single_click_subscribe(BUTTON_ID_UP, btn_up);
  window_single_click_subscribe(BUTTON_ID_DOWN, btn_down);
}

static void init() {
  selTimWindow = window_create();

  window_set_window_handlers(selTimWindow, (WindowHandlers) {
    .load = main_window_load,
  });
  window_set_click_config_provider(selTimWindow, clickConfig);

  window_stack_push(selTimWindow, true);
}

static void deinit() {
    window_destroy(selTimWindow);
}
int main(void) {
  init();
  app_event_loop();
  deinit();
}