/*************************************************************************/
/*                                                                       */
/*  Implementation of Clustergen, Statistical Parameter Synthesizer in   */
/*  Tritium                                                              */
/*                                                                       */
/*  A statistical corpus based synthesizer.                              */
/*  http://www.cs.cmu.edu/~awb/papers/is2006/IS061394.PDF                */
/*                                                                       */
/*  Uses MLSA for resynthesis and MLPG for smoothing                     */
/*  mlsa and mlpg come from Festvox's VC code (which came in turn        */
/*  came from NITECH's HTS                                               */
/*                                                                       */
/*************************************************************************/

#include "cst_cg.h"
#include "cst_hrg.h"
#include "cst_utt_utils.h"
#include "cst_audio.h"

CST_VAL_REGISTER_TYPE_NODEL(cg_db,cst_cg_db)

static cst_utterance *cg_make_hmmstates(cst_utterance *utt);
static cst_utterance *cg_make_params(cst_utterance *utt);
static cst_utterance *cg_predict_params(cst_utterance *utt);
static cst_utterance *cg_resynth(cst_utterance *utt);

cst_utterance *cg_synth(cst_utterance *utt)
{

    cg_make_hmmstates(utt);
    cg_make_params(utt);
    cg_predict_params(utt);
    cg_resynth(utt);

    return utt;
}

static float cg_state_duration(cst_item *s, cst_cg_db *cg_db)
{
    float zdur = val_float(cart_interpret(s,cg_db->dur_cart));
    const char *n = item_feat_string(s,"name");
	int i = 0;
	int x = 0;

    for (; cg_db->dur_stats[i]; ++i)
    {
        if (streq(cg_db->dur_stats[i]->phone, n))
        {
            x = i;
            break;
        }
    }
	if (!cg_db->dur_stats[i])  x = 0; // unknown type name
    float dur = (zdur * cg_db->dur_stats[x]->stddev) + cg_db->dur_stats[x]->mean;

    return dur;
}

static cst_utterance *cg_make_hmmstates(cst_utterance *utt)
{
    // build HMM state structure below the segment structure
    cst_cg_db *cg_db = val_cg_db(utt_feat_val(utt,"cg_db"));
    cst_relation *hmmstate = utt_relation_create(utt,"HMMstate");
	cst_relation *segstate = utt_relation_create(utt,"segstate");
	cst_item *s;
	cst_item *ss;
    const char *segname;
	int p;

    for (cst_item *seg = utt_rel_head(utt,"Segment"); seg; seg = item_next(seg))
    {
        ss = relation_append(segstate, seg);
        segname = item_feat_string(seg, "name");
        for (p = 0; cg_db->phone_states[p]; ++p)
		{
            if (streq(segname,cg_db->phone_states[p][0])) break;
		}
        if (!cg_db->phone_states[p]) p = 0;  // unknown phoneme
        for (int sp = 1; cg_db->phone_states[p][sp]; ++sp)
        {
            s = relation_append(hmmstate, NULL);
            item_add_daughter(ss, s);
            item_set_string(s, "name", cg_db->phone_states[p][sp]);
            item_set_int(s, "statepos", sp);
        }
    }

    return utt;
}

static cst_utterance *cg_make_params(cst_utterance *utt)
{
    // puts in the frame items
    // historically called "mcep" but can actually be any random vectors
    cst_cg_db *cg_db = val_cg_db(utt_feat_val(utt, "cg_db"));
    cst_relation *mcep = utt_relation_create(utt, "mcep");
	cst_relation *mcep_link = utt_relation_create(utt, "mcep_link");
    cst_item *s, *mcep_parent, *mcep_frame;
    int num_frames = 0;
	float start;
	float end = 0.0;
    float dur_stretch = get_param_float(utt->features, "duration_stretch", 1.0);

    for (s = utt_rel_head(utt, "HMMstate"); s; s = item_next(s))
    {
        start = end;
        end = start + (dur_stretch*cg_state_duration(s, cg_db));
        item_set_float(s, "end", end);
        mcep_parent = relation_append(mcep_link, s);
        for (; (num_frames * cg_db->frame_advance) <= end; ++num_frames )
        {
            mcep_frame = relation_append(mcep, NULL);
            item_add_daughter(mcep_parent, mcep_frame);
            item_set_int(mcep_frame, "frame_number", num_frames);
            item_set(mcep_frame, "name", item_feat(mcep_parent, "name"));
        }
    }

    // copy duration up onto Segment relation
    for (s = utt_rel_head(utt, "Segment"); s; s = item_next(s))
	{
		item_set(s, "end", ffeature(s, "R:segstate.daughtern.end"));
	}

    utt_set_feat_int(utt, "param_track_num_frames", num_frames);

    return utt;
}

#if CG_OLD
static int voiced_frame(cst_item *m)
{
    const char *ph_vc = ffeature_string(m, "R:mcep_link.parent.R:segstate.parent.ph_vc");
    const char *ph_cvox = ffeature_string(m, "R:mcep_link.parent.R:segstate.parent.ph_cvox");

    if (streq("-", ph_vc) && streq("-", ph_cvox)) return 0; // unvoiced
    else return 1; // voiced
}
#endif

static int voiced_frame(cst_item *m)
{
    const char *ph_vc = ffeature_string(m,"R:mcep_link.parent.R:segstate.parent.ph_vc");
    const char *ph_name = ffeature_string(m,"R:mcep_link.parent.R:segstate.parent.name");

    if (streq(ph_name,"pau")) return 0; // unvoiced
    else if (streq("+",ph_vc)) return 1; // voiced
	// even though the range is 0-10, I *do* mean 0.5
    else if (item_feat_float(m,"voicing") > 0.5) return 1; // voiced
    else return 0; // unvoiced
}

static void cg_smooth_F0(cst_utterance *utt,cst_cg_db *cg_db,
                         cst_track *param_track)
{
    /* smooth F0 and mark unnoived frames as 0.0 */
    cst_item *mcep;
    int i, c;
	float l = 0.0;
	float s;

    for (i = 0; i < param_track->num_frames - 1; ++i)
    {
        c = 0;
		s = 0;
        if (l > 0.0)
        {
            ++c;
			s += l;
        }
        if (param_track->frames[i + 1][0] > 0.0)
        {
            ++c;
			s += param_track->frames[i + 1][0];
        }
        l = param_track->frames[i][0];
        if (param_track->frames[i][0] > 0.0)
        {
            ++c;
			s += param_track->frames[i][0];
            param_track->frames[i][0] = s / c;
        }
    }

    float mean = get_param_float(utt->features,"int_f0_target_mean", cg_db->f0_mean);
    mean *= get_param_float(utt->features,"f0_shift", 1.0);
    float stddev = get_param_float(utt->features,"int_f0_target_stddev", cg_db->f0_stddev);
    
    for (i = 0, mcep=utt_rel_head(utt, "mcep"); mcep; ++i, mcep=item_next(mcep))
    {
		// scale the F0 -- which normally wont change it at all
        if (voiced_frame(mcep)) param_track->frames[i][0] = (((param_track->frames[i][0] - cg_db->f0_mean) / cg_db->f0_stddev) * stddev) + mean;
        else param_track->frames[i][0] = 0.0; // unvoice it
    }
}

static cst_utterance *cg_predict_params(cst_utterance *utt)
{
    cst_cg_db *cg_db = val_cg_db(utt_feat_val(utt,"cg_db"));
    cst_track *param_track = new_track();
    cst_track *str_track = NULL;
    cst_item *mcep;
    const cst_cart *mcep_tree, *f0_tree;
    int i,j,f,p,fd,o;
    const char *mname;
    float f0_val;
    int fff;
    int extra_feats = 1; // voicing

	// test what should be the default
    if (cg_db->do_mlpg) fff = 1;  // copy details with stddevs
    else fff = 2;  // copy details without stddevs

    if (cg_db->mixed_excitation)
    {
        extra_feats += 5;
        str_track = new_track();
        cst_track_resize(str_track, utt_feat_int(utt, "param_track_num_frames"), 5);
    }
    
    cst_track_resize(param_track, utt_feat_int(utt, "param_track_num_frames"), (cg_db->num_channels0 / fff) - (2 * extra_feats));
    for (i = 0, mcep = utt_rel_head(utt, "mcep"); mcep; ++i, mcep = item_next(mcep))
    {
        mname = item_feat_string(mcep, "name");
        for (p = 0; cg_db->types[p]; ++p)
		{
            if (streq(mname,cg_db->types[p])) break;
		}
        if (!cg_db->types[0]) p = 0; // if there isn't a matching tree, use the first one

        // predict F0
        f0_tree = cg_db->f0_trees[p];
        f0_val = val_float(cart_interpret(mcep, f0_tree));
        param_track->frames[i][0] = f0_val;
        // what about stddev?

        if (cg_db->multimodel)
        {
			// MULTI model
            f = val_int(cart_interpret(mcep, cg_db->param_trees0[p]));
            fd = val_int(cart_interpret(mcep, cg_db->param_trees1[p]));
            item_set_int(mcep, "clustergen_param_frame", f);

            param_track->frames[i][0] = (param_track->frames[i][0] + CG_MODEL_VECTOR(cg_db, model_vectors0, f, 0) + CG_MODEL_VECTOR(cg_db, model_vectors1, fd, 0)) / 3.0;
            for (j = 2; j < param_track->num_channels; ++j)
			{
                param_track->frames[i][j] = (CG_MODEL_VECTOR(cg_db, model_vectors0, f, (j) * fff) + CG_MODEL_VECTOR(cg_db, model_vectors1, fd, (j) * fff)) / 2.0;
			}
            if (cg_db->mixed_excitation)
            {
                o = j;
                for (j = 0; j < 5; ++j)
                {
                    str_track->frames[i][j] = (CG_MODEL_VECTOR(cg_db, model_vectors0, f, (o + (2 * j)) * fff) + CG_MODEL_VECTOR(cg_db, model_vectors1, fd, (o + (2 * j)) * fff)) / 2.0;
                }
            }
        }
        else  
        {
			// SINGLE model
            // predict spectral
            mcep_tree = cg_db->param_trees0[p];
            f = val_int(cart_interpret(mcep, mcep_tree));
            item_set_int(mcep, "clustergen_param_frame", f);

            param_track->frames[i][0] = (param_track->frames[i][0] + CG_MODEL_VECTOR(cg_db, model_vectors0, f, 0)) / 2.0;

            for (j = 2; j < param_track->num_channels; ++j)
			{
                param_track->frames[i][j] = CG_MODEL_VECTOR(cg_db, model_vectors0, f, j * fff);
			}

            if (cg_db->mixed_excitation)
            {
                o = j;
                for (j = 0; j < 5; ++j)
                {
                    str_track->frames[i][j] = CG_MODEL_VECTOR(cg_db, model_vectors0, f, (o + (2 * j)) *fff);
                }
            }
        }

        // last coefficient is average voicing for cluster
        item_set_float(mcep,"voicing", CG_MODEL_VECTOR(cg_db, model_vectors0, f, cg_db->num_channels0 - 2));
        param_track->times[i] = i * cg_db->frame_advance;
    }

    cg_smooth_F0(utt, cg_db, param_track);

    utt_set_feat(utt, "param_track", track_val(param_track));
    if (cg_db->mixed_excitation) utt_set_feat(utt, "str_track", track_val(str_track));

    return utt;
}

static cst_utterance *cg_resynth(cst_utterance *utt)
{
    cst_cg_db *cg_db = val_cg_db(utt_feat_val(utt, "cg_db"));
    cst_wave *w;
    cst_track *param_track = val_track(utt_feat_val(utt, "param_track"));
    cst_track *str_track = NULL;
    cst_track *smoothed_track;
    const cst_val *streaming_info_val = get_param_val(utt->features, "streaming_info", NULL);
    cst_audio_streaming_info *asi = NULL;

    if (streaming_info_val) asi = val_audio_streaming_info(streaming_info_val);
    if (cg_db->mixed_excitation) str_track = val_track(utt_feat_val(utt, "str_track"));

    if (cg_db->do_mlpg)
    {
        smoothed_track = mlpg(param_track, cg_db);
        w = mlsa_resynthesis(smoothed_track, str_track, cg_db, asi);
        delete_track(smoothed_track);
    }
    else w = mlsa_resynthesis(param_track, str_track, cg_db, asi);
    utt_set_wave(utt, w);

    return utt;
}
