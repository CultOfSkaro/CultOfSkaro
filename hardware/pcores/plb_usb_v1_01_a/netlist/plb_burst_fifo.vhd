--------------------------------------------------------------------------------
-- Copyright (c) 1995-2009 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____
--  /   /\/   /
-- /___/  \  /    Vendor: Xilinx
-- \   \   \/     Version: L.33
--  \   \         Application: netgen
--  /   /         Filename: plb_burst_fifo.vhd
-- /___/   /\     Timestamp: Tue Feb 09 17:39:11 2010
-- \   \  /  \ 
--  \___\/\___\
--             
-- Command	: -intstyle ise -w -sim -ofmt vhdl .\tmp\_cg\plb_burst_fifo.ngc .\tmp\_cg\plb_burst_fifo.vhd 
-- Device	: 4vfx20ff672-10
-- Input file	: ./tmp/_cg/plb_burst_fifo.ngc
-- Output file	: ./tmp/_cg/plb_burst_fifo.vhd
-- # of Entities	: 1
-- Design Name	: plb_burst_fifo
-- Xilinx	: C:\Xilinx\11.1\ISE
--             
-- Purpose:    
--     This VHDL netlist is a verification model and uses simulation 
--     primitives which may not represent the true implementation of the 
--     device, however the netlist is functionally correct and should not 
--     be modified. This file cannot be synthesized and should only be used 
--     with supported simulation tools.
--             
-- Reference:  
--     Development System Reference Guide, Chapter 23
--     Synthesis and Simulation Design Guide, Chapter 6
--             
--------------------------------------------------------------------------------


-- synthesis translate_off
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
use UNISIM.VPKG.ALL;

entity plb_burst_fifo is
  port (
    rd_en : in STD_LOGIC := 'X'; 
    wr_en : in STD_LOGIC := 'X'; 
    full : out STD_LOGIC; 
    empty : out STD_LOGIC; 
    wr_clk : in STD_LOGIC := 'X'; 
    rst : in STD_LOGIC := 'X'; 
    rd_clk : in STD_LOGIC := 'X'; 
    dout : out STD_LOGIC_VECTOR ( 15 downto 0 ); 
    din : in STD_LOGIC_VECTOR ( 31 downto 0 ) 
  );
end plb_burst_fifo;

architecture STRUCTURE of plb_burst_fifo is
  signal BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000117_274 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000235_273 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or000035_272 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or000051_271 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000119_270 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000269_269 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000252_268 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000236_267 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000196_266 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000169_265 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or000081_264 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or000049_263 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or000015_262 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000170_261 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000237_260 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or000036_259 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000105_258 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000199_257 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000168_256 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000103_255 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or000067_254 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_wpntr_count_not0001 : STD_LOGIC; 
  signal BU2_U0_grf_rf_mem_tmp_ram_rd_en : STD_LOGIC; 
  signal BU2_N01 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_wpntr_N0 : STD_LOGIC; 
  signal BU2_N2 : STD_LOGIC; 
  signal BU2_U0_grf_rf_mem_dout_i_not0001 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count1 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count2 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count3 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count6 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count4 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count5 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_gr1_rfwft_empty_fwft_fb_199 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_gr1_rfwft_empty_fwft_i_mux0000 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_0_rt_190 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_1_rt_189 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_2_rt_187 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_3_rt_185 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_4_rt_183 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_5_rt_181 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_6_rt_179 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_xor_7_rt_177 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count1 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count2 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count3 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count4 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count5 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count6 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count7 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_xor0005 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_xor0004 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_xor0003 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_xor0002 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_xor0001 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_xor0000 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0006 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0005 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0004 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0003 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0002 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0001 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0000 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0005 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0004 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0003 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0002 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0001 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0000 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0005 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0004 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0003 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0002 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0001 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0000 : STD_LOGIC; 
  signal BU2_U0_grf_rf_rstblk_wr_rst_comb : STD_LOGIC; 
  signal BU2_U0_grf_rf_rstblk_rd_rst_comb : STD_LOGIC; 
  signal BU2_U0_grf_rf_rstblk_wr_rst_asreg_68 : STD_LOGIC; 
  signal BU2_U0_grf_rf_rstblk_rd_rst_asreg_67 : STD_LOGIC; 
  signal BU2_U0_grf_rf_rstblk_wr_rst_asreg_d2_66 : STD_LOGIC; 
  signal BU2_U0_grf_rf_rstblk_wr_rst_asreg_d1_65 : STD_LOGIC; 
  signal BU2_U0_grf_rf_rstblk_rd_rst_asreg_d2_64 : STD_LOGIC; 
  signal BU2_U0_grf_rf_rstblk_rd_rst_asreg_d1_63 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_fb_i_62 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_59 : STD_LOGIC; 
  signal BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000 : STD_LOGIC; 
  signal BU2_N1 : STD_LOGIC; 
  signal NLW_VCC_P_UNCONNECTED : STD_LOGIC; 
  signal NLW_GND_G_UNCONNECTED : STD_LOGIC; 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_CASCADEOUTA_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_CASCADEOUTB_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_31_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_30_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_29_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_28_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_27_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_26_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_25_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_24_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_23_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_22_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_21_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_20_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_19_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_18_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_17_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_16_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_15_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_14_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_13_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_12_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_11_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_10_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_9_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_8_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_7_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_6_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_5_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_4_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_3_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_2_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_1_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_0_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_31_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_30_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_29_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_28_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_27_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_26_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_25_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_24_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_23_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_22_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_21_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_20_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_19_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_18_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_17_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_16_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOPA_3_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOPA_2_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOPA_1_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOPA_0_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOPB_3_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOPB_2_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOPB_1_UNCONNECTED : STD_LOGIC;
 
  signal NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOPB_0_UNCONNECTED : STD_LOGIC;
 
  signal din_2 : STD_LOGIC_VECTOR ( 31 downto 0 ); 
  signal dout_3 : STD_LOGIC_VECTOR ( 15 downto 0 ); 
  signal BU2_U0_grf_rf_mem_dout_mem : STD_LOGIC_VECTOR ( 15 downto 0 ); 
  signal BU2_U0_grf_rf_gl0_wr_wpntr_count_d2 : STD_LOGIC_VECTOR ( 6 downto 0 ); 
  signal BU2_U0_grf_rf_gl0_wr_wpntr_count_d1 : STD_LOGIC_VECTOR ( 6 downto 0 ); 
  signal BU2_U0_grf_rf_gl0_wr_wpntr_count : STD_LOGIC_VECTOR ( 6 downto 0 ); 
  signal BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state_mux0001 : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_count_d1 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy : STD_LOGIC_VECTOR ( 6 downto 0 ); 
  signal BU2_U0_grf_rf_gl0_rd_rpntr_count : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc : STD_LOGIC_VECTOR ( 6 downto 0 ); 
  signal BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1 : STD_LOGIC_VECTOR ( 7 downto 1 ); 
  signal BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1 : STD_LOGIC_VECTOR ( 6 downto 0 ); 
  signal BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg : STD_LOGIC_VECTOR ( 6 downto 0 ); 
  signal BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin : STD_LOGIC_VECTOR ( 6 downto 0 ); 
  signal BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin : STD_LOGIC_VECTOR ( 7 downto 1 ); 
  signal BU2_U0_grf_rf_rstblk_wr_rst_reg : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal BU2_U0_grf_rf_rstblk_rd_rst_reg : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal BU2_rd_data_count : STD_LOGIC_VECTOR ( 0 downto 0 ); 
begin
  dout(15) <= dout_3(15);
  dout(14) <= dout_3(14);
  dout(13) <= dout_3(13);
  dout(12) <= dout_3(12);
  dout(11) <= dout_3(11);
  dout(10) <= dout_3(10);
  dout(9) <= dout_3(9);
  dout(8) <= dout_3(8);
  dout(7) <= dout_3(7);
  dout(6) <= dout_3(6);
  dout(5) <= dout_3(5);
  dout(4) <= dout_3(4);
  dout(3) <= dout_3(3);
  dout(2) <= dout_3(2);
  dout(1) <= dout_3(1);
  dout(0) <= dout_3(0);
  din_2(31) <= din(31);
  din_2(30) <= din(30);
  din_2(29) <= din(29);
  din_2(28) <= din(28);
  din_2(27) <= din(27);
  din_2(26) <= din(26);
  din_2(25) <= din(25);
  din_2(24) <= din(24);
  din_2(23) <= din(23);
  din_2(22) <= din(22);
  din_2(21) <= din(21);
  din_2(20) <= din(20);
  din_2(19) <= din(19);
  din_2(18) <= din(18);
  din_2(17) <= din(17);
  din_2(16) <= din(16);
  din_2(15) <= din(15);
  din_2(14) <= din(14);
  din_2(13) <= din(13);
  din_2(12) <= din(12);
  din_2(11) <= din(11);
  din_2(10) <= din(10);
  din_2(9) <= din(9);
  din_2(8) <= din(8);
  din_2(7) <= din(7);
  din_2(6) <= din(6);
  din_2(5) <= din(5);
  din_2(4) <= din(4);
  din_2(3) <= din(3);
  din_2(2) <= din(2);
  din_2(1) <= din(1);
  din_2(0) <= din(0);
  VCC_0 : VCC
    port map (
      P => NLW_VCC_P_UNCONNECTED
    );
  GND_1 : GND
    port map (
      G => NLW_GND_G_UNCONNECTED
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count_xor_6_1_SW0 : LUT4_L
    generic map(
      INIT => X"7FFF"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count(0),
      I1 => BU2_U0_grf_rf_gl0_wr_wpntr_count(1),
      I2 => BU2_U0_grf_rf_gl0_wr_wpntr_count(2),
      I3 => BU2_U0_grf_rf_gl0_wr_wpntr_count(3),
      LO => BU2_N01
    );
  BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000105 : LUT4_L
    generic map(
      INIT => X"9000"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(5),
      I1 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(6),
      I2 => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or000067_254,
      I3 => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000103_255,
      LO => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000105_258
    );
  BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000252 : LUT3_L
    generic map(
      INIT => X"09"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(2),
      I1 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(1),
      I2 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(0),
      LO => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000252_268
    );
  BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000117 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count(5),
      I1 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(4),
      I2 => BU2_U0_grf_rf_gl0_rd_rpntr_count(4),
      I3 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(3),
      LO => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000117_274
    );
  BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or000051 : LUT3_L
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or000015_262,
      I1 => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or000049_263,
      I2 => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001,
      LO => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or000051_271
    );
  BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000235 : LUT4_L
    generic map(
      INIT => X"8421"
    )
    port map (
      I0 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(3),
      I1 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(4),
      I2 => BU2_U0_grf_rf_gl0_wr_wpntr_count(2),
      I3 => BU2_U0_grf_rf_gl0_wr_wpntr_count(3),
      LO => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000235_273
    );
  BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or000035 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(0),
      I1 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(1),
      I2 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(2),
      I3 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(3),
      LO => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or000035_272
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count_xor_3_121 : LUT3_D
    generic map(
      INIT => X"7F"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count(0),
      I1 => BU2_U0_grf_rf_gl0_wr_wpntr_count(1),
      I2 => BU2_U0_grf_rf_gl0_wr_wpntr_count(2),
      LO => BU2_N2,
      O => BU2_U0_grf_rf_gl0_wr_wpntr_N0
    );
  BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP : RAMB16
    generic map(
      DOA_REG => 0,
      DOB_REG => 0,
      INIT_A => X"000000000",
      INIT_B => X"000000000",
      INITP_00 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INITP_01 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INITP_02 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INITP_03 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INITP_04 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INITP_05 => X"0000000000000000000000000000000000000000000000000000000000000000",
      SRVAL_A => X"000000000",
      INIT_00 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_01 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_02 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_03 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_04 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_05 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_06 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_07 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_08 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_09 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_0A => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_0B => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_0C => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_0D => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_0E => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_0F => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_10 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_11 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_12 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_13 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_14 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_15 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_16 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_17 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_18 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_19 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_1A => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_1B => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_1C => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_1D => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_1E => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_1F => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_20 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_21 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_22 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_23 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_24 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_25 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_26 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_27 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_28 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_29 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_2A => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_2B => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_2C => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_2D => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_2E => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_2F => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_30 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_31 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_32 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_33 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_34 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_35 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_36 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_37 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_38 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_39 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_3A => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_3B => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_3C => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_3D => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_3E => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_3F => X"0000000000000000000000000000000000000000000000000000000000000000",
      INIT_FILE => "NONE",
      INVERT_CLK_DOA_REG => FALSE,
      INVERT_CLK_DOB_REG => FALSE,
      RAM_EXTENSION_A => "NONE",
      RAM_EXTENSION_B => "NONE",
      READ_WIDTH_A => 18,
      READ_WIDTH_B => 18,
      SIM_COLLISION_CHECK => "ALL",
      INITP_06 => X"0000000000000000000000000000000000000000000000000000000000000000",
      INITP_07 => X"0000000000000000000000000000000000000000000000000000000000000000",
      WRITE_MODE_A => "WRITE_FIRST",
      WRITE_MODE_B => "WRITE_FIRST",
      WRITE_WIDTH_A => 36,
      WRITE_WIDTH_B => 36,
      SRVAL_B => X"000000000"
    )
    port map (
      CASCADEINA => BU2_rd_data_count(0),
      CASCADEINB => BU2_rd_data_count(0),
      CLKA => wr_clk,
      CLKB => rd_clk,
      ENA => BU2_N1,
      REGCEA => BU2_rd_data_count(0),
      REGCEB => BU2_rd_data_count(0),
      ENB => BU2_U0_grf_rf_mem_tmp_ram_rd_en,
      SSRA => BU2_rd_data_count(0),
      SSRB => BU2_U0_grf_rf_rstblk_rd_rst_reg(0),
      CASCADEOUTA => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_CASCADEOUTA_UNCONNECTED,
      CASCADEOUTB => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_CASCADEOUTB_UNCONNECTED,
      ADDRA(14) => BU2_rd_data_count(0),
      ADDRA(13) => BU2_rd_data_count(0),
      ADDRA(12) => BU2_rd_data_count(0),
      ADDRA(11) => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(6),
      ADDRA(10) => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(5),
      ADDRA(9) => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(4),
      ADDRA(8) => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(3),
      ADDRA(7) => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(2),
      ADDRA(6) => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(1),
      ADDRA(5) => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(0),
      ADDRA(4) => BU2_rd_data_count(0),
      ADDRA(3) => BU2_rd_data_count(0),
      ADDRA(2) => BU2_rd_data_count(0),
      ADDRA(1) => BU2_rd_data_count(0),
      ADDRA(0) => BU2_rd_data_count(0),
      ADDRB(14) => BU2_rd_data_count(0),
      ADDRB(13) => BU2_rd_data_count(0),
      ADDRB(12) => BU2_rd_data_count(0),
      ADDRB(11) => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(7),
      ADDRB(10) => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(6),
      ADDRB(9) => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(5),
      ADDRB(8) => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(4),
      ADDRB(7) => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(3),
      ADDRB(6) => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(2),
      ADDRB(5) => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(1),
      ADDRB(4) => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(0),
      ADDRB(3) => BU2_rd_data_count(0),
      ADDRB(2) => BU2_rd_data_count(0),
      ADDRB(1) => BU2_rd_data_count(0),
      ADDRB(0) => BU2_rd_data_count(0),
      DIA(31) => din_2(15),
      DIA(30) => din_2(14),
      DIA(29) => din_2(13),
      DIA(28) => din_2(12),
      DIA(27) => din_2(11),
      DIA(26) => din_2(10),
      DIA(25) => din_2(9),
      DIA(24) => din_2(8),
      DIA(23) => din_2(7),
      DIA(22) => din_2(6),
      DIA(21) => din_2(5),
      DIA(20) => din_2(4),
      DIA(19) => din_2(3),
      DIA(18) => din_2(2),
      DIA(17) => din_2(1),
      DIA(16) => din_2(0),
      DIA(15) => din_2(31),
      DIA(14) => din_2(30),
      DIA(13) => din_2(29),
      DIA(12) => din_2(28),
      DIA(11) => din_2(27),
      DIA(10) => din_2(26),
      DIA(9) => din_2(25),
      DIA(8) => din_2(24),
      DIA(7) => din_2(23),
      DIA(6) => din_2(22),
      DIA(5) => din_2(21),
      DIA(4) => din_2(20),
      DIA(3) => din_2(19),
      DIA(2) => din_2(18),
      DIA(1) => din_2(17),
      DIA(0) => din_2(16),
      DIB(31) => BU2_rd_data_count(0),
      DIB(30) => BU2_rd_data_count(0),
      DIB(29) => BU2_rd_data_count(0),
      DIB(28) => BU2_rd_data_count(0),
      DIB(27) => BU2_rd_data_count(0),
      DIB(26) => BU2_rd_data_count(0),
      DIB(25) => BU2_rd_data_count(0),
      DIB(24) => BU2_rd_data_count(0),
      DIB(23) => BU2_rd_data_count(0),
      DIB(22) => BU2_rd_data_count(0),
      DIB(21) => BU2_rd_data_count(0),
      DIB(20) => BU2_rd_data_count(0),
      DIB(19) => BU2_rd_data_count(0),
      DIB(18) => BU2_rd_data_count(0),
      DIB(17) => BU2_rd_data_count(0),
      DIB(16) => BU2_rd_data_count(0),
      DIB(15) => BU2_rd_data_count(0),
      DIB(14) => BU2_rd_data_count(0),
      DIB(13) => BU2_rd_data_count(0),
      DIB(12) => BU2_rd_data_count(0),
      DIB(11) => BU2_rd_data_count(0),
      DIB(10) => BU2_rd_data_count(0),
      DIB(9) => BU2_rd_data_count(0),
      DIB(8) => BU2_rd_data_count(0),
      DIB(7) => BU2_rd_data_count(0),
      DIB(6) => BU2_rd_data_count(0),
      DIB(5) => BU2_rd_data_count(0),
      DIB(4) => BU2_rd_data_count(0),
      DIB(3) => BU2_rd_data_count(0),
      DIB(2) => BU2_rd_data_count(0),
      DIB(1) => BU2_rd_data_count(0),
      DIB(0) => BU2_rd_data_count(0),
      DIPA(3) => BU2_rd_data_count(0),
      DIPA(2) => BU2_rd_data_count(0),
      DIPA(1) => BU2_rd_data_count(0),
      DIPA(0) => BU2_rd_data_count(0),
      DIPB(3) => BU2_rd_data_count(0),
      DIPB(2) => BU2_rd_data_count(0),
      DIPB(1) => BU2_rd_data_count(0),
      DIPB(0) => BU2_rd_data_count(0),
      WEA(3) => BU2_U0_grf_rf_gl0_wr_wpntr_count_not0001,
      WEA(2) => BU2_U0_grf_rf_gl0_wr_wpntr_count_not0001,
      WEA(1) => BU2_U0_grf_rf_gl0_wr_wpntr_count_not0001,
      WEA(0) => BU2_U0_grf_rf_gl0_wr_wpntr_count_not0001,
      WEB(3) => BU2_rd_data_count(0),
      WEB(2) => BU2_rd_data_count(0),
      WEB(1) => BU2_rd_data_count(0),
      WEB(0) => BU2_rd_data_count(0),
      DOA(31) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_31_UNCONNECTED,
      DOA(30) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_30_UNCONNECTED,
      DOA(29) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_29_UNCONNECTED,
      DOA(28) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_28_UNCONNECTED,
      DOA(27) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_27_UNCONNECTED,
      DOA(26) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_26_UNCONNECTED,
      DOA(25) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_25_UNCONNECTED,
      DOA(24) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_24_UNCONNECTED,
      DOA(23) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_23_UNCONNECTED,
      DOA(22) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_22_UNCONNECTED,
      DOA(21) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_21_UNCONNECTED,
      DOA(20) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_20_UNCONNECTED,
      DOA(19) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_19_UNCONNECTED,
      DOA(18) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_18_UNCONNECTED,
      DOA(17) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_17_UNCONNECTED,
      DOA(16) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_16_UNCONNECTED,
      DOA(15) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_15_UNCONNECTED,
      DOA(14) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_14_UNCONNECTED,
      DOA(13) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_13_UNCONNECTED,
      DOA(12) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_12_UNCONNECTED,
      DOA(11) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_11_UNCONNECTED,
      DOA(10) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_10_UNCONNECTED,
      DOA(9) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_9_UNCONNECTED,
      DOA(8) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_8_UNCONNECTED,
      DOA(7) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_7_UNCONNECTED,
      DOA(6) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_6_UNCONNECTED,
      DOA(5) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_5_UNCONNECTED,
      DOA(4) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_4_UNCONNECTED,
      DOA(3) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_3_UNCONNECTED,
      DOA(2) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_2_UNCONNECTED,
      DOA(1) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_1_UNCONNECTED,
      DOA(0) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOA_0_UNCONNECTED,
      DOB(31) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_31_UNCONNECTED,
      DOB(30) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_30_UNCONNECTED,
      DOB(29) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_29_UNCONNECTED,
      DOB(28) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_28_UNCONNECTED,
      DOB(27) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_27_UNCONNECTED,
      DOB(26) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_26_UNCONNECTED,
      DOB(25) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_25_UNCONNECTED,
      DOB(24) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_24_UNCONNECTED,
      DOB(23) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_23_UNCONNECTED,
      DOB(22) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_22_UNCONNECTED,
      DOB(21) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_21_UNCONNECTED,
      DOB(20) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_20_UNCONNECTED,
      DOB(19) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_19_UNCONNECTED,
      DOB(18) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_18_UNCONNECTED,
      DOB(17) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_17_UNCONNECTED,
      DOB(16) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOB_16_UNCONNECTED,
      DOB(15) => BU2_U0_grf_rf_mem_dout_mem(15),
      DOB(14) => BU2_U0_grf_rf_mem_dout_mem(14),
      DOB(13) => BU2_U0_grf_rf_mem_dout_mem(13),
      DOB(12) => BU2_U0_grf_rf_mem_dout_mem(12),
      DOB(11) => BU2_U0_grf_rf_mem_dout_mem(11),
      DOB(10) => BU2_U0_grf_rf_mem_dout_mem(10),
      DOB(9) => BU2_U0_grf_rf_mem_dout_mem(9),
      DOB(8) => BU2_U0_grf_rf_mem_dout_mem(8),
      DOB(7) => BU2_U0_grf_rf_mem_dout_mem(7),
      DOB(6) => BU2_U0_grf_rf_mem_dout_mem(6),
      DOB(5) => BU2_U0_grf_rf_mem_dout_mem(5),
      DOB(4) => BU2_U0_grf_rf_mem_dout_mem(4),
      DOB(3) => BU2_U0_grf_rf_mem_dout_mem(3),
      DOB(2) => BU2_U0_grf_rf_mem_dout_mem(2),
      DOB(1) => BU2_U0_grf_rf_mem_dout_mem(1),
      DOB(0) => BU2_U0_grf_rf_mem_dout_mem(0),
      DOPA(3) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOPA_3_UNCONNECTED,
      DOPA(2) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOPA_2_UNCONNECTED,
      DOPA(1) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOPA_1_UNCONNECTED,
      DOPA(0) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOPA_0_UNCONNECTED,
      DOPB(3) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOPB_3_UNCONNECTED,
      DOPB(2) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOPB_2_UNCONNECTED,
      DOPB(1) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOPB_1_UNCONNECTED,
      DOPB(0) => 
NLW_BU2_U0_grf_rf_mem_gbm_gbmg_gbmga_ngecc_bmg_blk_mem_generator_valid_cstr_ramloop_0_ram_r_v4_noinit_ram_SDP_SINGLE_PRIM_SDP_DOPB_0_UNCONNECTED
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count_xor_0_11_INV_0 : INV
    port map (
      I => BU2_U0_grf_rf_gl0_wr_wpntr_count(0),
      O => BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count
    );
  BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => wr_en,
      I1 => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_fb_i_62,
      O => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count_xor_3_11 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count(0),
      I1 => BU2_U0_grf_rf_gl0_wr_wpntr_count(3),
      I2 => BU2_U0_grf_rf_gl0_wr_wpntr_count(1),
      I3 => BU2_U0_grf_rf_gl0_wr_wpntr_count(2),
      O => BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count3
    );
  BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000119 : LUT4
    generic map(
      INIT => X"9000"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count(3),
      I1 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(2),
      I2 => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or000081_264,
      I3 => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000117_274,
      O => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000119_270
    );
  BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000237 : LUT4
    generic map(
      INIT => X"9000"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count(5),
      I1 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(6),
      I2 => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000199_257,
      I3 => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000235_273,
      O => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000237_260
    );
  BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000170 : LUT4
    generic map(
      INIT => X"9000"
    )
    port map (
      I0 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(2),
      I1 => BU2_U0_grf_rf_gl0_wr_wpntr_count(1),
      I2 => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000168_256,
      I3 => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      O => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000170_261
    );
  BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or000036 : LUT3
    generic map(
      INIT => X"90"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(6),
      I1 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(7),
      I2 => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or000035_272,
      O => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or000036_259
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_xor_7_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count(7),
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_xor_7_rt_177
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_0_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count(0),
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_0_rt_190
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_1_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count(1),
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_1_rt_189
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_2_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count(2),
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_2_rt_187
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_3_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count(3),
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_3_rt_185
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_4_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count(4),
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_4_rt_183
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_5_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count(5),
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_5_rt_181
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_6_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count(6),
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_6_rt_179
    );
  BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000280 : LUT3
    generic map(
      INIT => X"EC"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000119_270,
      I1 => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000269_269,
      I2 => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or000051_271,
      O => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000
    );
  BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000269 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000252_268,
      I1 => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000236_267,
      I2 => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000196_266,
      I3 => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000169_265,
      O => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000269_269
    );
  BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000236 : LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(6),
      I1 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(5),
      I2 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(3),
      I3 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(2),
      O => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000236_267
    );
  BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000196 : LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(4),
      I1 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(3),
      I2 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(7),
      I3 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(6),
      O => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000196_266
    );
  BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000169 : LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(1),
      I1 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(0),
      I2 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(5),
      I3 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(4),
      O => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000169_265
    );
  BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or000081 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count(1),
      I1 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(0),
      O => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or000081_264
    );
  BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or000049 : LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count(2),
      I1 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(1),
      I2 => BU2_U0_grf_rf_gl0_rd_rpntr_count(6),
      I3 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(5),
      O => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or000049_263
    );
  BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or000015 : LUT3
    generic map(
      INIT => X"09"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count(7),
      I1 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(6),
      I2 => BU2_U0_grf_rf_gl0_rd_rpntr_count(0),
      O => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or000015_262
    );
  BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000262 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000105_258,
      I1 => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or000036_259,
      I2 => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000237_260,
      I3 => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000170_261,
      O => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000
    );
  BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000199 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count(4),
      I1 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(5),
      O => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000199_257
    );
  BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000168 : LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count(6),
      I1 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(7),
      I2 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(1),
      I3 => BU2_U0_grf_rf_gl0_wr_wpntr_count(0),
      O => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000168_256
    );
  BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000103 : LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(3),
      I1 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(4),
      I2 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(1),
      I3 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(2),
      O => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000103_255
    );
  BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or000067 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(4),
      I1 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(5),
      O => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or000067_254
    );
  BU2_U0_grf_rf_gl0_rd_gr1_rfwft_Mmux_RAM_RD_EN_FWFT21 : LUT4
    generic map(
      INIT => X"2333"
    )
    port map (
      I0 => rd_en,
      I1 => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_59,
      I2 => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state(1),
      I3 => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state(0),
      O => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001
    );
  BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => wr_en,
      I1 => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_fb_i_62,
      O => BU2_U0_grf_rf_gl0_wr_wpntr_count_not0001
    );
  BU2_U0_grf_rf_mem_tmp_ram_rd_en1 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => BU2_U0_grf_rf_rstblk_rd_rst_reg(0),
      I1 => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001,
      O => BU2_U0_grf_rf_mem_tmp_ram_rd_en
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count_xor_6_1 : LUT4
    generic map(
      INIT => X"CC6C"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count(5),
      I1 => BU2_U0_grf_rf_gl0_wr_wpntr_count(6),
      I2 => BU2_U0_grf_rf_gl0_wr_wpntr_count(4),
      I3 => BU2_N01,
      O => BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count6
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count_xor_5_11 : LUT4
    generic map(
      INIT => X"CC6C"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count(4),
      I1 => BU2_U0_grf_rf_gl0_wr_wpntr_count(5),
      I2 => BU2_U0_grf_rf_gl0_wr_wpntr_count(3),
      I3 => BU2_U0_grf_rf_gl0_wr_wpntr_N0,
      O => BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count5
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor00051 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(1),
      I1 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(2),
      I2 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(3),
      I3 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0002,
      O => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0005
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor00051 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(0),
      I1 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(1),
      I2 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(2),
      I3 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0002,
      O => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0005
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count_xor_4_11 : LUT3
    generic map(
      INIT => X"C6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count(3),
      I1 => BU2_U0_grf_rf_gl0_wr_wpntr_count(4),
      I2 => BU2_N2,
      O => BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count4
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor00041 : LUT3
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(2),
      I1 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(3),
      I2 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0002,
      O => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0004
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor00041 : LUT3
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(1),
      I1 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(2),
      I2 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0002,
      O => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0004
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor00032 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(3),
      I1 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0002,
      O => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0003
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor00032 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(2),
      I1 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0002,
      O => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0003
    );
  BU2_U0_grf_rf_gl0_rd_gr1_rfwft_Mmux_RAM_REGOUT_EN11 : LUT3
    generic map(
      INIT => X"A2"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state(1),
      I1 => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state(0),
      I2 => rd_en,
      O => BU2_U0_grf_rf_mem_dout_i_not0001
    );
  BU2_U0_grf_rf_gl0_rd_gr1_rfwft_empty_fwft_i_mux00001 : LUT4
    generic map(
      INIT => X"8E8A"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_empty_fwft_fb_199,
      I1 => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state(0),
      I2 => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state(1),
      I3 => rd_en,
      O => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_empty_fwft_i_mux0000
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor00021 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(4),
      I1 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(5),
      I2 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(6),
      I3 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(7),
      O => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0002
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor00021 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(3),
      I1 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(4),
      I2 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(5),
      I3 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(6),
      O => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0002
    );
  BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state_mux0001_0_1 : LUT4
    generic map(
      INIT => X"08FF"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state(1),
      I1 => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state(0),
      I2 => rd_en,
      I3 => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_59,
      O => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state_mux0001(0)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count_xor_2_11 : LUT3
    generic map(
      INIT => X"6C"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count(0),
      I1 => BU2_U0_grf_rf_gl0_wr_wpntr_count(2),
      I2 => BU2_U0_grf_rf_gl0_wr_wpntr_count(1),
      O => BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count2
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor00011 : LUT3
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(7),
      I1 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(6),
      I2 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(5),
      O => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0001
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor00011 : LUT3
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(6),
      I1 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(5),
      I2 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(4),
      O => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0001
    );
  BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state_mux0001_1_1 : LUT3
    generic map(
      INIT => X"F2"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state(0),
      I1 => rd_en,
      I2 => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state(1),
      O => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state_mux0001(1)
    );
  BU2_U0_grf_rf_gcx_clkx_Mxor_rd_pntr_gc_xor0000_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(7),
      I1 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(6),
      O => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0000
    );
  BU2_U0_grf_rf_gcx_clkx_Mxor_rd_pntr_gc_xor0001_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(6),
      I1 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(5),
      O => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0001
    );
  BU2_U0_grf_rf_gcx_clkx_Mxor_rd_pntr_gc_xor0002_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(5),
      I1 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(4),
      O => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0002
    );
  BU2_U0_grf_rf_gcx_clkx_Mxor_rd_pntr_gc_xor0003_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(4),
      I1 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(3),
      O => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0003
    );
  BU2_U0_grf_rf_gcx_clkx_Mxor_rd_pntr_gc_xor0004_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(3),
      I1 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(2),
      O => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0004
    );
  BU2_U0_grf_rf_gcx_clkx_Mxor_rd_pntr_gc_xor0005_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(2),
      I1 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(1),
      O => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0005
    );
  BU2_U0_grf_rf_gcx_clkx_Mxor_rd_pntr_gc_xor0006_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(1),
      I1 => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(0),
      O => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0006
    );
  BU2_U0_grf_rf_gcx_clkx_Mxor_wr_pntr_gc_xor0000_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(6),
      I1 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(5),
      O => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_xor0000
    );
  BU2_U0_grf_rf_gcx_clkx_Mxor_wr_pntr_gc_xor0001_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(5),
      I1 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(4),
      O => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_xor0001
    );
  BU2_U0_grf_rf_gcx_clkx_Mxor_wr_pntr_gc_xor0002_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(4),
      I1 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(3),
      O => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_xor0002
    );
  BU2_U0_grf_rf_gcx_clkx_Mxor_wr_pntr_gc_xor0003_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(3),
      I1 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(2),
      O => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_xor0003
    );
  BU2_U0_grf_rf_gcx_clkx_Mxor_wr_pntr_gc_xor0004_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(2),
      I1 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(1),
      O => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_xor0004
    );
  BU2_U0_grf_rf_gcx_clkx_Mxor_wr_pntr_gc_xor0005_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(1),
      I1 => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(0),
      O => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_xor0005
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count_xor_1_11 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gl0_wr_wpntr_count(1),
      I1 => BU2_U0_grf_rf_gl0_wr_wpntr_count(0),
      O => BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count1
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor00001 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(6),
      I1 => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(7),
      O => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0000
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor00001 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(5),
      I1 => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(6),
      O => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0000
    );
  BU2_U0_grf_rf_rstblk_rd_rst_comb1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => BU2_U0_grf_rf_rstblk_rd_rst_asreg_67,
      I1 => BU2_U0_grf_rf_rstblk_rd_rst_asreg_d2_64,
      O => BU2_U0_grf_rf_rstblk_rd_rst_comb
    );
  BU2_U0_grf_rf_rstblk_wr_rst_comb1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => BU2_U0_grf_rf_rstblk_wr_rst_asreg_68,
      I1 => BU2_U0_grf_rf_rstblk_wr_rst_asreg_d2_66,
      O => BU2_U0_grf_rf_rstblk_wr_rst_comb
    );
  BU2_U0_grf_rf_mem_dout_i_0 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_mem_dout_i_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(0),
      D => BU2_U0_grf_rf_mem_dout_mem(0),
      Q => dout_3(0)
    );
  BU2_U0_grf_rf_mem_dout_i_1 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_mem_dout_i_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(0),
      D => BU2_U0_grf_rf_mem_dout_mem(1),
      Q => dout_3(1)
    );
  BU2_U0_grf_rf_mem_dout_i_2 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_mem_dout_i_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(0),
      D => BU2_U0_grf_rf_mem_dout_mem(2),
      Q => dout_3(2)
    );
  BU2_U0_grf_rf_mem_dout_i_3 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_mem_dout_i_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(0),
      D => BU2_U0_grf_rf_mem_dout_mem(3),
      Q => dout_3(3)
    );
  BU2_U0_grf_rf_mem_dout_i_4 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_mem_dout_i_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(0),
      D => BU2_U0_grf_rf_mem_dout_mem(4),
      Q => dout_3(4)
    );
  BU2_U0_grf_rf_mem_dout_i_5 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_mem_dout_i_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(0),
      D => BU2_U0_grf_rf_mem_dout_mem(5),
      Q => dout_3(5)
    );
  BU2_U0_grf_rf_mem_dout_i_6 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_mem_dout_i_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(0),
      D => BU2_U0_grf_rf_mem_dout_mem(6),
      Q => dout_3(6)
    );
  BU2_U0_grf_rf_mem_dout_i_7 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_mem_dout_i_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(0),
      D => BU2_U0_grf_rf_mem_dout_mem(7),
      Q => dout_3(7)
    );
  BU2_U0_grf_rf_mem_dout_i_8 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_mem_dout_i_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(0),
      D => BU2_U0_grf_rf_mem_dout_mem(8),
      Q => dout_3(8)
    );
  BU2_U0_grf_rf_mem_dout_i_9 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_mem_dout_i_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(0),
      D => BU2_U0_grf_rf_mem_dout_mem(9),
      Q => dout_3(9)
    );
  BU2_U0_grf_rf_mem_dout_i_10 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_mem_dout_i_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(0),
      D => BU2_U0_grf_rf_mem_dout_mem(10),
      Q => dout_3(10)
    );
  BU2_U0_grf_rf_mem_dout_i_11 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_mem_dout_i_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(0),
      D => BU2_U0_grf_rf_mem_dout_mem(11),
      Q => dout_3(11)
    );
  BU2_U0_grf_rf_mem_dout_i_12 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_mem_dout_i_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(0),
      D => BU2_U0_grf_rf_mem_dout_mem(12),
      Q => dout_3(12)
    );
  BU2_U0_grf_rf_mem_dout_i_13 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_mem_dout_i_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(0),
      D => BU2_U0_grf_rf_mem_dout_mem(13),
      Q => dout_3(13)
    );
  BU2_U0_grf_rf_mem_dout_i_14 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_mem_dout_i_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(0),
      D => BU2_U0_grf_rf_mem_dout_mem(14),
      Q => dout_3(14)
    );
  BU2_U0_grf_rf_mem_dout_i_15 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_mem_dout_i_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(0),
      D => BU2_U0_grf_rf_mem_dout_mem(15),
      Q => dout_3(15)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_d2_0 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(0),
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(0)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_d2_1 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(1),
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(1)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_d2_2 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(2),
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(2)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_d2_3 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(3),
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(3)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_d2_4 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(4),
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(4)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_d2_5 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(5),
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(5)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_d2_6 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(6),
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(6)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_d1_5 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_count(5),
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(5)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_d1_4 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_count(4),
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(4)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_d1_6 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_count(6),
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(6)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_d1_3 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_count(3),
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(3)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_d1_2 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_count(2),
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(2)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_d1_1 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_count(1),
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(1)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_d1_0 : FDPE
    generic map(
      INIT => '1'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      D => BU2_U0_grf_rf_gl0_wr_wpntr_count(0),
      PRE => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count_d1(0)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_0 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count,
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count(0)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_1 : FDPE
    generic map(
      INIT => '1'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      D => BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count1,
      PRE => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count(1)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_2 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count2,
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count(2)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_3 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count3,
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count(3)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_6 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count6,
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count(6)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_4 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count4,
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count(4)
    );
  BU2_U0_grf_rf_gl0_wr_wpntr_count_5 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_gl0_wr_ram_wr_en_i1_204,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_Mcount_count5,
      Q => BU2_U0_grf_rf_gl0_wr_wpntr_count(5)
    );
  BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state_0 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      D => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state_mux0001(1),
      Q => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state(0)
    );
  BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state_1 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      D => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state_mux0001(0),
      Q => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_curr_fwft_state(1)
    );
  BU2_U0_grf_rf_gl0_rd_gr1_rfwft_empty_fwft_i : FDP
    generic map(
      INIT => '1'
    )
    port map (
      C => rd_clk,
      D => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_empty_fwft_i_mux0000,
      PRE => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      Q => empty
    );
  BU2_U0_grf_rf_gl0_rd_gr1_rfwft_empty_fwft_fb : FDP
    generic map(
      INIT => '1'
    )
    port map (
      C => rd_clk,
      D => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_empty_fwft_i_mux0000,
      PRE => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      Q => BU2_U0_grf_rf_gl0_rd_gr1_rfwft_empty_fwft_fb_199
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_count_d1_0 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      D => BU2_U0_grf_rf_gl0_rd_rpntr_count(0),
      Q => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(0)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_count_d1_1 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      D => BU2_U0_grf_rf_gl0_rd_rpntr_count(1),
      Q => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(1)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_count_d1_2 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      D => BU2_U0_grf_rf_gl0_rd_rpntr_count(2),
      Q => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(2)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_count_d1_3 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      D => BU2_U0_grf_rf_gl0_rd_rpntr_count(3),
      Q => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(3)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_count_d1_4 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      D => BU2_U0_grf_rf_gl0_rd_rpntr_count(4),
      Q => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(4)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_count_d1_5 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      D => BU2_U0_grf_rf_gl0_rd_rpntr_count(5),
      Q => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(5)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_count_d1_6 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      D => BU2_U0_grf_rf_gl0_rd_rpntr_count(6),
      Q => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(6)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_count_d1_7 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      D => BU2_U0_grf_rf_gl0_rd_rpntr_count(7),
      Q => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(7)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_0_Q : MUXCY
    port map (
      CI => BU2_N1,
      DI => BU2_rd_data_count(0),
      S => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_0_rt_190,
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(0)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_xor_0_Q : XORCY
    port map (
      CI => BU2_N1,
      LI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_0_rt_190,
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_1_Q : MUXCY
    port map (
      CI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(0),
      DI => BU2_rd_data_count(0),
      S => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_1_rt_189,
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(1)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_xor_1_Q : XORCY
    port map (
      CI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(0),
      LI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_1_rt_189,
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count1
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_2_Q : MUXCY
    port map (
      CI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(1),
      DI => BU2_rd_data_count(0),
      S => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_2_rt_187,
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(2)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_xor_2_Q : XORCY
    port map (
      CI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(1),
      LI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_2_rt_187,
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count2
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_3_Q : MUXCY
    port map (
      CI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(2),
      DI => BU2_rd_data_count(0),
      S => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_3_rt_185,
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(3)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_xor_3_Q : XORCY
    port map (
      CI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(2),
      LI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_3_rt_185,
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count3
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_4_Q : MUXCY
    port map (
      CI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(3),
      DI => BU2_rd_data_count(0),
      S => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_4_rt_183,
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(4)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_xor_4_Q : XORCY
    port map (
      CI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(3),
      LI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_4_rt_183,
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count4
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_5_Q : MUXCY
    port map (
      CI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(4),
      DI => BU2_rd_data_count(0),
      S => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_5_rt_181,
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(5)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_xor_5_Q : XORCY
    port map (
      CI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(4),
      LI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_5_rt_181,
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count5
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_6_Q : MUXCY
    port map (
      CI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(5),
      DI => BU2_rd_data_count(0),
      S => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_6_rt_179,
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(6)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_xor_6_Q : XORCY
    port map (
      CI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(5),
      LI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy_6_rt_179,
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count6
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_xor_7_Q : XORCY
    port map (
      CI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_cy(6),
      LI => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count_xor_7_rt_177,
      O => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count7
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_count_0 : FDPE
    generic map(
      INIT => '1'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001,
      D => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count,
      PRE => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      Q => BU2_U0_grf_rf_gl0_rd_rpntr_count(0)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_count_1 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      D => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count1,
      Q => BU2_U0_grf_rf_gl0_rd_rpntr_count(1)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_count_2 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      D => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count2,
      Q => BU2_U0_grf_rf_gl0_rd_rpntr_count(2)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_count_3 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      D => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count3,
      Q => BU2_U0_grf_rf_gl0_rd_rpntr_count(3)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_count_4 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      D => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count4,
      Q => BU2_U0_grf_rf_gl0_rd_rpntr_count(4)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_count_5 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      D => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count5,
      Q => BU2_U0_grf_rf_gl0_rd_rpntr_count(5)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_count_6 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      D => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count6,
      Q => BU2_U0_grf_rf_gl0_rd_rpntr_count(6)
    );
  BU2_U0_grf_rf_gl0_rd_rpntr_count_7 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_gl0_rd_rpntr_count_not0001,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      D => BU2_U0_grf_rf_gl0_rd_rpntr_Mcount_count7,
      Q => BU2_U0_grf_rf_gl0_rd_rpntr_count(7)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_0 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_xor0005,
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc(0)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_1 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_xor0004,
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc(1)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_2 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_xor0003,
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc(2)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_3 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_xor0002,
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc(3)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_4 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_xor0001,
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc(4)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_5 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_xor0000,
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc(5)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_6 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gl0_wr_wpntr_count_d2(6),
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc(6)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_0 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0006,
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc(0)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_1 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0005,
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc(1)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_2 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0004,
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc(2)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_3 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0003,
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc(3)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_4 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0002,
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc(4)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_5 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0001,
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc(5)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_6 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_xor0000,
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc(6)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_7 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_rd_rpntr_count_d1(7),
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc(7)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_0 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc(0),
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg(0)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_1 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc(1),
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg(1)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_2 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc(2),
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg(2)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_3 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc(3),
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg(3)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_4 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc(4),
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg(4)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_5 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc(5),
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg(5)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_6 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc(6),
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg(6)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_7 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc(7),
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg(7)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_0 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc(0),
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg(0)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_1 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc(1),
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg(1)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_2 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc(2),
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg(2)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_3 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc(3),
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg(3)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_4 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc(4),
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg(4)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_5 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc(5),
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg(5)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_6 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc(6),
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg(6)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1_1 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg(1),
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(1)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1_2 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg(2),
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(2)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1_3 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg(3),
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(3)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1_4 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg(4),
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(4)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1_5 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg(5),
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(5)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1_6 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg(6),
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(6)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1_7 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg(7),
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(7)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1_0 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg(0),
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(0)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1_1 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg(1),
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(1)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1_2 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg(2),
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(2)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1_3 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg(3),
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(3)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1_4 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg(4),
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(4)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1_5 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg(5),
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(5)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1_6 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg(6),
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(6)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_0 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0005,
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(0)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_1 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0004,
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(1)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_2 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0003,
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(2)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_3 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0002,
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(3)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_4 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0001,
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(4)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_5 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_xor0000,
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(5)
    );
  BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin_6 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CLR => BU2_U0_grf_rf_rstblk_rd_rst_reg(1),
      D => BU2_U0_grf_rf_gcx_clkx_wr_pntr_gc_asreg_d1(6),
      Q => BU2_U0_grf_rf_gcx_clkx_wr_pntr_bin(6)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_1 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0005,
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(1)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_2 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0004,
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(2)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_3 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0003,
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(3)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_4 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0002,
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(4)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_5 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0001,
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(5)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_6 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_xor0000,
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(6)
    );
  BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin_7 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(0),
      D => BU2_U0_grf_rf_gcx_clkx_rd_pntr_gc_asreg_d1(7),
      Q => BU2_U0_grf_rf_gcx_clkx_rd_pntr_bin(7)
    );
  BU2_U0_grf_rf_rstblk_wr_rst_reg_0 : FDP
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      D => BU2_rd_data_count(0),
      PRE => BU2_U0_grf_rf_rstblk_wr_rst_comb,
      Q => BU2_U0_grf_rf_rstblk_wr_rst_reg(0)
    );
  BU2_U0_grf_rf_rstblk_wr_rst_reg_1 : FDP
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      D => BU2_rd_data_count(0),
      PRE => BU2_U0_grf_rf_rstblk_wr_rst_comb,
      Q => BU2_U0_grf_rf_rstblk_wr_rst_reg(1)
    );
  BU2_U0_grf_rf_rstblk_rd_rst_reg_0 : FDP
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      D => BU2_rd_data_count(0),
      PRE => BU2_U0_grf_rf_rstblk_rd_rst_comb,
      Q => BU2_U0_grf_rf_rstblk_rd_rst_reg(0)
    );
  BU2_U0_grf_rf_rstblk_rd_rst_reg_1 : FDP
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      D => BU2_rd_data_count(0),
      PRE => BU2_U0_grf_rf_rstblk_rd_rst_comb,
      Q => BU2_U0_grf_rf_rstblk_rd_rst_reg(1)
    );
  BU2_U0_grf_rf_rstblk_rd_rst_reg_2 : FDP
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      D => BU2_rd_data_count(0),
      PRE => BU2_U0_grf_rf_rstblk_rd_rst_comb,
      Q => BU2_U0_grf_rf_rstblk_rd_rst_reg(2)
    );
  BU2_U0_grf_rf_rstblk_rd_rst_asreg : FDPE
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      CE => BU2_U0_grf_rf_rstblk_rd_rst_asreg_d1_63,
      D => BU2_rd_data_count(0),
      PRE => rst,
      Q => BU2_U0_grf_rf_rstblk_rd_rst_asreg_67
    );
  BU2_U0_grf_rf_rstblk_wr_rst_asreg_d1 : FD
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      D => BU2_U0_grf_rf_rstblk_wr_rst_asreg_68,
      Q => BU2_U0_grf_rf_rstblk_wr_rst_asreg_d1_65
    );
  BU2_U0_grf_rf_rstblk_wr_rst_asreg : FDPE
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CE => BU2_U0_grf_rf_rstblk_wr_rst_asreg_d1_65,
      D => BU2_rd_data_count(0),
      PRE => rst,
      Q => BU2_U0_grf_rf_rstblk_wr_rst_asreg_68
    );
  BU2_U0_grf_rf_rstblk_rd_rst_asreg_d1 : FD
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      D => BU2_U0_grf_rf_rstblk_rd_rst_asreg_67,
      Q => BU2_U0_grf_rf_rstblk_rd_rst_asreg_d1_63
    );
  BU2_U0_grf_rf_rstblk_wr_rst_asreg_d2 : FD
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      D => BU2_U0_grf_rf_rstblk_wr_rst_asreg_d1_65,
      Q => BU2_U0_grf_rf_rstblk_wr_rst_asreg_d2_66
    );
  BU2_U0_grf_rf_rstblk_rd_rst_asreg_d2 : FD
    generic map(
      INIT => '0'
    )
    port map (
      C => rd_clk,
      D => BU2_U0_grf_rf_rstblk_rd_rst_asreg_d1_63,
      Q => BU2_U0_grf_rf_rstblk_rd_rst_asreg_d2_64
    );
  BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_fb_i : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000,
      Q => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_fb_i_62
    );
  BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i : FDC
    generic map(
      INIT => '0'
    )
    port map (
      C => wr_clk,
      CLR => BU2_U0_grf_rf_rstblk_wr_rst_reg(1),
      D => BU2_U0_grf_rf_gl0_wr_gwas_wsts_ram_full_i_or0000,
      Q => full
    );
  BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i : FDP
    generic map(
      INIT => '1'
    )
    port map (
      C => rd_clk,
      D => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_or0000,
      PRE => BU2_U0_grf_rf_rstblk_rd_rst_reg(2),
      Q => BU2_U0_grf_rf_gl0_rd_gras_rsts_ram_empty_fb_i_59
    );
  BU2_XST_VCC : VCC
    port map (
      P => BU2_N1
    );
  BU2_XST_GND : GND
    port map (
      G => BU2_rd_data_count(0)
    );

end STRUCTURE;

-- synthesis translate_on
