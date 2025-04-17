#ifndef __EPDFONT_H
#define __EPDFONT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tl_common.h"


typedef struct {
  const uint8_t *table;
  uint16_t      width;
  uint16_t      height;
} sFont;

extern sFont font68;
extern sFont font58;
extern sFont font32;
extern sFont font24;
extern sFont font26;
extern sFont font20;
extern sFont font16;
extern sFont font13;
extern sFont font12;
extern sFont font8;


#ifdef __cplusplus
}
#endif

#endif
