#ifndef SRC_INCLUDE_APP_BUTTON_H_
#define SRC_INCLUDE_APP_BUTTON_H_

#define DEBOUNCE_BUTTON             3      /* number of polls for debounce                 */

extern bool factory_reset;

void button_handler(void);
int32_t button_handlerCb(void *args);
bool button_idle();

#endif /* SRC_INCLUDE_APP_BUTTON_H_ */
