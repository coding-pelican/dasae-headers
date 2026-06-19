#include "dansi-dec/model.h"

fn_((dansi_dec_model_feats(dansi_dec_model_Term term))(dansi_dec_model_Feats) $scope) {
    switch (term) {
    case_((dansi_dec_model_Term_vt52)) return l$((dansi_dec_model_Feats){
        .has_csi_8bit = false,
        .has_s8c1t = false,
        .has_left_right_margin = false,
        .has_rectangular_ops = false,
    }) $end(case);
    case_((dansi_dec_model_Term_vt100)) return l$((dansi_dec_model_Feats){
        .has_csi_8bit = false,
        .has_s8c1t = false,
        .has_left_right_margin = false,
        .has_rectangular_ops = false,
    }) $end(case);
    case_((dansi_dec_model_Term_vt220)) return l$((dansi_dec_model_Feats){
        .has_csi_8bit = true,
        .has_s8c1t = true,
        .has_left_right_margin = false,
        .has_rectangular_ops = false,
    }) $end(case);
    default_((
        dansi_dec_model_Term_vt320,
        dansi_dec_model_Term_vt420,
        dansi_dec_model_Term_vt510,
        dansi_dec_model_Term_vt520
    )) return l$((dansi_dec_model_Feats){
        .has_csi_8bit = true,
        .has_s8c1t = true,
        .has_left_right_margin = true,
        .has_rectangular_ops = true,
    }) $end(default);
    };
} $unscoped(fn);
