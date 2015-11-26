/* Generated automatically from make_voice_list */

#include "tritium.h"

cst_voice *register_cmu_us_slt(const char *voxdir);

cst_val *tritium_set_voice_list(void)
{
   tritium_voiceList = cons_val(voice_val(register_cmu_us_slt(NULL)),tritium_voiceList);
   tritium_voiceList = val_reverse(tritium_voiceList);
   return tritium_voiceList;
}

