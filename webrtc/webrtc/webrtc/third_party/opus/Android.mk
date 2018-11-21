LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libopus

LOCAL_C_INCLUDES := $(JNI_PATH)/webrtc \
					$(LOCAL_PATH)/src/include \
					$(LOCAL_PATH)/src/celt \
					$(LOCAL_PATH)/src/silk \
					$(LOCAL_PATH)/src/silk/float

LOCAL_CFLAGS += -DHAVE_LRINT -DHAVE_LRINTF -DVAR_ARRAYS -DOPUS_BUILD
					
LOCAL_SRC_FILES := src/celt/bands.c \
				   src/celt/celt.c \
				   src/celt/celt_decoder.c \
				   src/celt/celt_encoder.c \
				   src/celt/celt_lpc.c \
				   src/celt/cwrs.c \
				   src/celt/entcode.c \
				   src/celt/entdec.c \
				   src/celt/entenc.c \
				   src/celt/kiss_fft.c \
				   src/celt/laplace.c \
				   src/celt/mathops.c \
				   src/celt/mdct.c \
				   src/celt/modes.c \
				   src/celt/pitch.c \
				   src/celt/quant_bands.c \
				   src/celt/rate.c \
				   src/celt/vq.c \
				   src/silk/A2NLSF.c \
				   src/silk/CNG.c \
				   src/silk/HP_variable_cutoff.c \
				   src/silk/LPC_analysis_filter.c \
				   src/silk/LPC_fit.c \
				   src/silk/LPC_inv_pred_gain.c \
				   src/silk/LP_variable_cutoff.c \
				   src/silk/NLSF2A.c \
				   src/silk/NLSF_VQ.c \
				   src/silk/NLSF_VQ_weights_laroia.c \
				   src/silk/NLSF_decode.c \
				   src/silk/NLSF_del_dec_quant.c \
				   src/silk/NLSF_encode.c \
				   src/silk/NLSF_stabilize.c \
				   src/silk/NLSF_unpack.c \
				   src/silk/NSQ.c \
				   src/silk/NSQ_del_dec.c \
				   src/silk/PLC.c \
				   src/silk/VAD.c \
				   src/silk/VQ_WMat_EC.c \
				   src/silk/ana_filt_bank_1.c \
				   src/silk/biquad_alt.c \
				   src/silk/bwexpander.c \
				   src/silk/bwexpander_32.c \
				   src/silk/check_control_input.c \
				   src/silk/code_signs.c \
				   src/silk/control_SNR.c \
				   src/silk/control_audio_bandwidth.c \
				   src/silk/control_codec.c \
				   src/silk/debug.c \
				   src/silk/dec_API.c \
				   src/silk/decode_core.c \
				   src/silk/decode_frame.c \
				   src/silk/decode_indices.c \
				   src/silk/decode_parameters.c \
				   src/silk/decode_pitch.c \
				   src/silk/decode_pulses.c \
				   src/silk/decoder_set_fs.c \
				   src/silk/enc_API.c \
				   src/silk/encode_indices.c \
				   src/silk/encode_pulses.c \
				   src/silk/gain_quant.c \
				   src/silk/init_decoder.c \
				   src/silk/init_encoder.c \
				   src/silk/inner_prod_aligned.c \
				   src/silk/interpolate.c \
				   src/silk/lin2log.c \
				   src/silk/log2lin.c \
				   src/silk/pitch_est_tables.c \
				   src/silk/process_NLSFs.c \
				   src/silk/quant_LTP_gains.c \
				   src/silk/resampler.c \
				   src/silk/resampler_down2.c \
				   src/silk/resampler_down2_3.c \
				   src/silk/resampler_private_AR2.c \
				   src/silk/resampler_private_IIR_FIR.c \
				   src/silk/resampler_private_down_FIR.c \
				   src/silk/resampler_private_up2_HQ.c \
				   src/silk/resampler_rom.c \
				   src/silk/shell_coder.c \
				   src/silk/sigm_Q15.c \
				   src/silk/sort.c \
				   src/silk/stereo_LR_to_MS.c \
				   src/silk/stereo_MS_to_LR.c \
				   src/silk/stereo_decode_pred.c \
				   src/silk/stereo_encode_pred.c \
				   src/silk/stereo_find_predictor.c \
				   src/silk/stereo_quant_pred.c \
				   src/silk/sum_sqr_shift.c \
				   src/silk/table_LSF_cos.c \
				   src/silk/tables_LTP.c \
				   src/silk/tables_NLSF_CB_NB_MB.c \
				   src/silk/tables_NLSF_CB_WB.c \
				   src/silk/tables_gain.c \
				   src/silk/tables_other.c \
				   src/silk/tables_pitch_lag.c \
				   src/silk/tables_pulses_per_block.c \
				   src/silk/float/LPC_analysis_filter_FLP.c \
				   src/silk/float/LPC_inv_pred_gain_FLP.c \
				   src/silk/float/LTP_analysis_filter_FLP.c \
				   src/silk/float/LTP_scale_ctrl_FLP.c \
				   src/silk/float/apply_sine_window_FLP.c \
				   src/silk/float/autocorrelation_FLP.c \
				   src/silk/float/burg_modified_FLP.c \
				   src/silk/float/bwexpander_FLP.c \
				   src/silk/float/corrMatrix_FLP.c \
				   src/silk/float/encode_frame_FLP.c \
				   src/silk/float/energy_FLP.c \
				   src/silk/float/find_LPC_FLP.c \
				   src/silk/float/find_LTP_FLP.c \
				   src/silk/float/find_pitch_lags_FLP.c \
				   src/silk/float/find_pred_coefs_FLP.c \
				   src/silk/float/inner_product_FLP.c \
				   src/silk/float/k2a_FLP.c \
				   src/silk/float/noise_shape_analysis_FLP.c \
				   src/silk/float/pitch_analysis_core_FLP.c \
				   src/silk/float/process_gains_FLP.c \
				   src/silk/float/regularize_correlations_FLP.c \
				   src/silk/float/residual_energy_FLP.c \
				   src/silk/float/scale_copy_vector_FLP.c \
				   src/silk/float/scale_vector_FLP.c \
				   src/silk/float/schur_FLP.c \
				   src/silk/float/sort_FLP.c \
				   src/silk/float/warped_autocorrelation_FLP.c \
				   src/silk/float/wrappers_FLP.c \
				   src/src/analysis.c \
				   src/src/mlp.c \
				   src/src/mlp_data.c \
				   src/src/opus.c \
				   src/src/opus_decoder.c \
				   src/src/opus_encoder.c \
				   src/src/opus_multistream.c \
				   src/src/opus_multistream_decoder.c \
				   src/src/opus_multistream_encoder.c \
				   src/src/repacketizer.c


include $(BUILD_STATIC_LIBRARY)