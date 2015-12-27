#ifndef _CST_CG_H__
#define _CST_CG_H__

#include "cst_cart.h"
#include "cst_track.h"
#include "cst_wave.h"
#include "cst_audio.h"
#include "cst_synth.h" /* for dur_stat */

typedef struct cst_cg_db_struct {
    const char *name;
    const char * const *types;
    int num_types;

    const int sample_rate;

    float f0_mean, f0_stddev;

    /* Cluster trees */
    const cst_cart * const *f0_trees; 
    const cst_cart * const *param_trees0; /* single or static */
    const cst_cart * const *param_trees1; /* deltas */
    const cst_cart * const *param_trees2; /* me str */

    /* Model params e.g. mceps, deltas intersliced with stddevs */
    const int num_channels0;
    const int num_frames0;
    const unsigned short * const * model_vectors0;

    const int num_channels1;
    const int num_frames1;
    const unsigned short * const * model_vectors1;

    const int num_channels2;
    const int num_frames2;
    const unsigned short * const * model_vectors2;

    /* Currently shared between different models */
    const float *model_min;    /* for vector coeffs encoding */
    const float *model_range;  /* for vector coeffs encoding */

    const float frame_advance; 

    /* duration model (cart + phonedurs) */
    const dur_stat * const *dur_stats;
    const cst_cart *dur_cart;

    /* phone to states map */
    const char * const * const *phone_states;

    /* Other parameters */    
    const int do_mlpg;  /* implies deltas are in the model_vectors */
    const float *dynwin;
    const int dynwinsize;

    const float mlsa_alpha;
    const float mlsa_beta;

    const int multimodel;
    const int mixed_excitation;

    /* filters for Mixed Excitation */
    const int ME_num;
    const int ME_order;
    const double * const *me_h;  

    const float gain;

} cst_cg_db;

/* Access model parameters, unpacking them as required */
#define CG_MODEL_VECTOR(M,N,X,Y)                                        \
    (M->model_min[Y]+((float)(M->N[X][Y])/65535.0*M->model_range[Y]))

CST_VAL_USER_TYPE_DCLS(cg_db,cst_cg_db)

cst_utterance *cg_synth(cst_utterance *utt);
cst_wave *mlsa_resynthesis(const cst_track *t, 
                           const cst_track *str, 
                           cst_cg_db *cg_db,
                           cst_audio_streaming_info *asc);
cst_track *mlpg(const cst_track *param_track, cst_cg_db *cg_db);

#endif
