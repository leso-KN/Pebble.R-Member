#include <pebble.h>
#include <TimSel.h>
#include <Notification.h>

static Window *selMsgWindow;
static TextLayer *selMsg_BG;
static TextLayer *selMsg_label;
static GFont fnt_txt;
static char curText[64];
static int actChr = 0;
static char * avlbleChrs=" abcdefghijklmnopqrstuvwxyz1234567890öüäß";
static int maxChars = 11;
static int curTextLen=0;
static char noChar;

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

static void renderText()
{
  char tmpTxt[64];
  int tmpIdx=0;
  for (int tmpPntr=0; tmpPntr<curTextLen; tmpPntr++)
  {
    if (curTextLen-tmpPntr<maxChars)
    {
      tmpTxt[tmpIdx]=curText[tmpPntr];
      tmpIdx++;
    }
  }
  tmpTxt[tmpIdx]=avlbleChrs[actChr];
  char * tmpTxtDst="";
  strcpy(tmpTxtDst,tmpTxt);
  text_layer_set_text(selMsg_label, tmpTxtDst);
}

static void main_window_load(Window *window) {
  selMsg_BG = text_layer_create(GRect(0,0,144,164));
  text_layer_set_background_color(selMsg_BG, GColorBlack);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(selMsg_BG));
  
  selMsg_label = text_layer_create(GRect(0, 55, 100, 37));
    
  text_layer_set_background_color(selMsg_label, GColorBlack);
  text_layer_set_text_color(selMsg_label, GColorWhite);
  
  renderText();
  
  text_layer_set_text_alignment(selMsg_label, GTextAlignmentRight);

  fnt_txt = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LATO_TXT_NORMAL_18));
  
  text_layer_set_font(selMsg_label, fnt_txt);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(selMsg_label));
}

void deinit_MsgSel() {
    window_destroy(selMsgWindow);
}

static void btn_select()
{
  curText[curTextLen]=avlbleChrs[actChr];
  curTextLen++;
  actChr=0;
  renderText();
}

static void btn_back()
{
  if (curTextLen<=maxChars) {
    showTimSel();
  }
  else
  {
    curTextLen--;
    curText[curTextLen]=noChar;
    actChr=0;
    renderText();
  }
}

void wakeup_handler(WakeupId id, int32_t noteID) {
  char * tmpMsg="";
  persist_read_string(noteID,tmpMsg,64);
  persist_delete(noteID);
  vibes_double_pulse();
  //notify(tmpMsg);
  window_stack_pop_all(true);
}

static void btn_accept()
{
  deinit();
  
  
  int32_t cnt=persist_read_int((int32_t)79560500000);
  cnt++;
  char tmpTxt[64];
  int tmpIdx=0;
  for (int tmpPntr=0; tmpPntr<curTextLen; tmpPntr++)
  {
    if (curTextLen-tmpPntr<maxChars)
    {
      tmpTxt[tmpIdx]=curText[tmpPntr];
      tmpIdx++;
    }
  }
  persist_write_string(79560500000+cnt,tmpTxt);
  wakeup_schedule(selTim_ticks, 79560500000+cnt, true);
  persist_write_int((int32_t)79560500000,cnt);
  
}
static void btn_up()
{
  actChr-=1;
  renderText();
}

static void btn_down()
{
  actChr+=1;
  renderText();
}

static void doNothing() { }

static void clickConfig()
{
  window_long_click_subscribe(BUTTON_ID_SELECT,500, btn_accept,doNothing);
  window_single_click_subscribe(BUTTON_ID_BACK, btn_back);
  window_single_click_subscribe(BUTTON_ID_SELECT, btn_select);
  window_single_click_subscribe(BUTTON_ID_UP, btn_up);
  window_single_click_subscribe(BUTTON_ID_DOWN, btn_down);
}

void selMsg()
{
  curTextLen=maxChars;
  for (int tmpPointr=0;tmpPointr<maxChars;tmpPointr++)
  {
    curText[tmpPointr]=avlbleChrs[0];
  }
  selMsgWindow = window_create();

  window_set_window_handlers(selMsgWindow, (WindowHandlers) {
    .load = main_window_load,
  });
  window_set_click_config_provider(selMsgWindow, clickConfig);
  window_stack_push(selMsgWindow, true);
}