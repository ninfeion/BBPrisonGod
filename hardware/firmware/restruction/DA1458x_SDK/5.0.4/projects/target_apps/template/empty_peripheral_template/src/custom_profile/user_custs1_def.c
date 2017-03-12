/**
 ****************************************************************************************
 *
 * @file user_custs1_def.c
 *
 * @brief Custom1 Server (CUSTS1) profile database definitions.
 *
 * Copyright (C) 2016. Dialog Semiconductor Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @defgroup USER_CONFIG
 * @ingroup USER
 * @brief Custom1 Server (CUSTS1) profile database definitions.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>
#include "prf_types.h"
#include "attm_db_128.h"
#include "user_custs1_def.h"

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
 
static att_svc_desc128_t custs1_svc                              = DEF_CUST1_SVC_UUID_128;

static uint8_t CUST1_BBPG_CONTROL_UUID_128[ATT_UUID_128_LEN]     = DEF_CUST1_BBPG_CONTROL_UUID_128;
static uint8_t CUST1_BBPG_LOSS_CHECK_UUID_128[ATT_UUID_128_LEN]  = DEF_CUST1_BBPG_LOSS_CHECK_UUID_128;
static uint8_t CUST1_BBPG_BREAK_CHECK_UUID_128[ATT_UUID_128_LEN] = DEF_CUST1_BBPG_BREAK_CHECK_UUID_128;

static struct att_char128_desc custs1_bbpg_control_char     = {ATT_CHAR_PROP_RD | ATT_CHAR_PROP_WR | ATT_CHAR_PROP_NTF,
                                                              {0, 0},
                                                              DEF_CUST1_BBPG_CONTROL_UUID_128};

static struct att_char128_desc custs1_bbpg_loss_check_char  = {ATT_CHAR_PROP_RD | ATT_CHAR_PROP_WR | ATT_CHAR_PROP_NTF,
                                                              {0, 0},
                                                              DEF_CUST1_BBPG_LOSS_CHECK_UUID_128};

static struct att_char128_desc custs1_bbpg_break_check_char = {ATT_CHAR_PROP_NTF,
                                                              {0, 0},
                                                              DEF_CUST1_BBPG_BREAK_CHECK_UUID_128};

static uint16_t att_decl_svc       = ATT_DECL_PRIMARY_SERVICE;
static uint16_t att_decl_char      = ATT_DECL_CHARACTERISTIC;
static uint16_t att_decl_cfg       = ATT_DESC_CLIENT_CHAR_CFG;
static uint16_t att_decl_user_desc = ATT_DESC_CHAR_USER_DESCRIPTION;

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Full CUSTOM1 Database Description - Used to add attributes into the database
struct attm_desc_128 custs1_att_db[CUST1_IDX_NB] = 
{
    // CUSTOM1 Service Declaration
    [CUST1_IDX_SVC]                     = {(uint8_t*)&att_decl_svc, ATT_UUID_16_LEN, PERM(RD, ENABLE),
                                            sizeof(custs1_svc), sizeof(custs1_svc), (uint8_t*)&custs1_svc},    

    [CUST1_IDX_BBPG_CONTROL_CHAR]       = {(uint8_t*)&att_decl_char, ATT_UUID_16_LEN, PERM(RD, ENABLE),
                                            sizeof(custs1_bbpg_control_char), sizeof(custs1_bbpg_control_char), (uint8_t*)&custs1_bbpg_control_char},
    [CUST1_IDX_BBPG_CONTROL_VAL]        = {CUST1_BBPG_CONTROL_UUID_128, ATT_UUID_128_LEN, PERM(RD, ENABLE) | PERM(WR, ENABLE) | PERM(NTF, ENABLE),
                                            DEF_CUST1_BBPG_CONTROL_CHAR_LEN, 0, NULL},    
    [CUST1_IDX_BBPG_CONTROL_NTF_CFG]    = {(uint8_t*)&att_decl_cfg, ATT_UUID_16_LEN, PERM(RD, ENABLE) | PERM(WR, ENABLE),
                                            sizeof(uint16_t), 0, NULL},                                                                                       
    [CUST1_IDX_BBPG_CONTROL_USER_DESC]  = {(uint8_t*)&att_decl_user_desc, ATT_UUID_16_LEN, PERM(RD, ENABLE),
                                            sizeof(CUST1_BBPG_CONTROL_USER_DESC) - 1, sizeof(CUST1_BBPG_CONTROL_USER_DESC) - 1, CUST1_BBPG_CONTROL_USER_DESC},
    
    [CUST1_IDX_BBPG_LOSS_CHECK_CHAR]     = {(uint8_t*)&att_decl_char, ATT_UUID_16_LEN, PERM(RD, ENABLE),
                                            sizeof(custs1_bbpg_loss_check_char), sizeof(custs1_bbpg_loss_check_char), (uint8_t*)&custs1_bbpg_loss_check_char},
    [CUST1_IDX_BBPG_LOSS_CHECK_VAL]      = {CUST1_BBPG_LOSS_CHECK_UUID_128, ATT_UUID_128_LEN, PERM(RD, ENABLE) | PERM(WR, ENABLE) | PERM(NTF, ENABLE),
                                            DEF_CUST1_BBPG_LOSS_CHECK_CHAR_LEN, 0, NULL},
    [CUST1_IDX_BBPG_LOSS_CHECK_NTF_CFG]  = {(uint8_t*)&att_decl_cfg, ATT_UUID_16_LEN, PERM(RD, ENABLE) | PERM(WR, ENABLE),
                                            sizeof(uint16_t), 0, NULL},
    [CUST1_IDX_BBPG_LOSS_CHECK_USER_DESC]= {(uint8_t*)&att_decl_user_desc, ATT_UUID_16_LEN, PERM(RD, ENABLE),
                                            sizeof(CUST1_BBPG_LOSS_CHECK_USER_DESC) - 1, sizeof(CUST1_BBPG_LOSS_CHECK_USER_DESC) - 1, CUST1_BBPG_LOSS_CHECK_USER_DESC},
    
    [CUST1_IDX_BBPG_BREAK_CHECK_CHAR]     = {(uint8_t*)&att_decl_char, ATT_UUID_16_LEN, PERM(RD, ENABLE),
                                            sizeof(custs1_bbpg_break_check_char), sizeof(custs1_bbpg_break_check_char), (uint8_t*)&custs1_bbpg_break_check_char},
    [CUST1_IDX_BBPG_BREAK_CHECK_VAL]      = {CUST1_BBPG_BREAK_CHECK_UUID_128, ATT_UUID_128_LEN, PERM(NTF, ENABLE),
                                            DEF_CUST1_BBPG_BREAK_CHECK_CHAR_LEN, 0, NULL},
    [CUST1_IDX_BBPG_BREAK_CHECK_NTF_CFG]  = {(uint8_t*)&att_decl_cfg, ATT_UUID_16_LEN, PERM(RD, ENABLE) | PERM(WR, ENABLE),
                                            sizeof(uint16_t), 0, NULL},
    [CUST1_IDX_BBPG_BREAK_CHECK_USER_DESC]= {(uint8_t*)&att_decl_user_desc, ATT_UUID_16_LEN, PERM(RD, ENABLE),
                                            sizeof(CUST1_BBPG_BREAK_CHECK_USER_DESC) - 1, sizeof(CUST1_BBPG_BREAK_CHECK_USER_DESC) - 1, CUST1_BBPG_BREAK_CHECK_USER_DESC},                                            
 
};

/// @} USER_CONFIG
