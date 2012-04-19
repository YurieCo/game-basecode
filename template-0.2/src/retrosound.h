#ifndef __RETROSOUND_H__
#define __RETROSOUND_H__

#include "retrocommon.h"

void RSO_Init(void);
void RSO_PlaySound(RetroSnd_t *s);
void RSO_PlayLoop(RetroSnd_t *s);
void RSO_StopLoop(void);

#endif /* __RETROSOUND_H__ */
