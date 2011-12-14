#!/bin/sh

export model_342="W342_step.msh"
export model_361="W361_step.msh"


export cnt_342="\
        W342_c0_ex_m2_t1\
	W342_c0_ex_m2_t2\
	W342_c0_ex_m2_t3\
	W342_c0_im_m2_t1\
	W342_c0_im_m2_t2\
	W342_c0_im_m2_t3"
	
export cnt_361="\
        W361_c0_ex_m2_t1\
	W361_c0_ex_m2_t2\
	W361_c0_ex_m2_t3\
	W361_c0_im_m2_t1\
	W361_c0_im_m2_t2\
	W361_c0_im_m2_t3"
	

../test_dyna_sub.sh $*


