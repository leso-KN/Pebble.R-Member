#include <pebble.h>
#include <Notification.h>

static Window *notiWindow;
static TextLayer *noti_BG;
static TextLayer *noti_tx_MSG;
static GFont fnt_noti;
static char * msg="Wecker abgelaufen!";

static void main_window_load(Window *window) {
  noti_BG = text_layer_create(GRect(0,0,144,164));
  text_layer_set_background_color(noti_BG, GColorBlack);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(noti_BG));
  
  noti_tx_MSG = text_layer_create(GRect(30, 52, 35, 37));
  
  text_layer_set_background_color(noti_tx_MSG, GColorBlack);
  text_layer_set_text_color(noti_tx_MSG, GColorWhite);
  
  text_layer_set_text(noti_tx_MSG, msg);
  text_layer_set_text_alignment(noti_tx_MSG, GTextAlignmentCenter);
  
  fnt_noti = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LATO_TXT_NORMAL_18));
  text_layer_set_font(noti_tx_MSG, fnt_noti);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(noti_tx_MSG));
}


static void btn_up()
{
  window_stack_pop_all(true);
}

static void btn_down()
{
  window_stack_pop_all(true);
}

static void clickConfig(){
  window_single_click_subscribe(BUTTON_ID_UP, btn_up);
  window_single_click_subscribe(BUTTON_ID_DOWN, btn_down);
}

void notify(char * n_msg) {
  msg = n_msg;
  notiWindow = window_create();

  window_set_window_handlers(notiWindow, (WindowHandlers) {
    .load = main_window_load,
  });
  window_set_click_config_provider(notiWindow, clickConfig);

  window_stack_push(notiWindow, true);
}