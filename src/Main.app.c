#include <pebble.h>
#include <TimSel.h>
#include <MsgSel.h>

int main(void) {
  if (launch_reason() == APP_LAUNCH_WAKEUP)
  {
    WakeupId id = 0;
    int32_t reason = 0;
    if (wakeup_get_launch_event(&id, &reason)) {
      wakeup_handler(id, reason);
    }
  }
  init();
  app_event_loop();
  deinit();
}