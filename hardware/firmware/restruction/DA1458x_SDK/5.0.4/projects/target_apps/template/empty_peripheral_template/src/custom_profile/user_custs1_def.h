/**
 ****************************************************************************************
 *
 * @file user_custs1_def.h
 *
 * @brief Custom1 Server (CUSTS1) profile database declarations.
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

#ifndef _USER_CUSTS1_DEF_H_
#define _USER_CUSTS1_DEF_H_

/**
 ****************************************************************************************
 * @defgroup USER_CONFIG
 * @ingroup USER
 * @brief Custom1 Server (CUSTS1) profile database declarations.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "attm_db_128.h"

#define DEF_CUST1_SVC_UUID_128              {0x2F, 0x2A, 0x93, 0xA6, 0xBD, 0xD8, 0x41, 0x52, 0xAC, 0x0B, 0x10, 0x99, 0x2E, 0xC6, 0xFE, 0xED}

#define DEF_CUST1_BBPG_CONTROL_UUID_128     {0x20, 0xEE, 0x8D, 0x0C, 0xE1, 0xF0, 0x4A, 0x0C, 0xB3, 0x25, 0xDC, 0x53, 0x6A, 0x68, 0x86, 0x2D}
#define DEF_CUST1_BBPG_LOSS_CHECK_UUID_128  {0x28, 0xD5, 0xE1, 0xC1, 0xE1, 0xC5, 0x47, 0x29, 0xB5, 0x57, 0x65, 0xC3, 0xBA, 0x47, 0x15, 0x9E}
#define DEF_CUST1_BBPG_BREAK_CHECK_UUID_128 {0x17, 0xB9, 0x67, 0x98, 0x4C, 0x66, 0x4C, 0x01, 0x96, 0x33, 0x31, 0xB1, 0x91, 0x59, 0x00, 0x15}

#define DEF_CUST1_BBPG_CONTROL_CHAR_LEN     32
#define DEF_CUST1_BBPG_LOSS_CHECK_CHAR_LEN  32
#define DEF_CUST1_BBPG_BREAK_CHECK_CHAR_LEN 32

#define CUST1_BBPG_CONTROL_USER_DESC      "BBPG CONTROL"
#define CUST1_BBPG_LOSS_CHECK_USER_DESC   "BBPG LOSS CHECK"
#define CUST1_BBPG_BREAK_CHECK_USER_DESC  "BBPG BREAK CHECK"

/*
 * DEFINES
 ****************************************************************************************
 */

/// Custom1 Service Data Base Characteristic enum
enum
{
    CUST1_IDX_SVC = 0,
    
    CUST1_IDX_BBPG_CONTROL_CHAR,
    CUST1_IDX_BBPG_CONTROL_VAL,
    CUST1_IDX_BBPG_CONTROL_NTF_CFG,    
    CUST1_IDX_BBPG_CONTROL_USER_DESC,
    
    CUST1_IDX_BBPG_LOSS_CHECK_CHAR,
    CUST1_IDX_BBPG_LOSS_CHECK_VAL,
    CUST1_IDX_BBPG_LOSS_CHECK_NTF_CFG,    
    CUST1_IDX_BBPG_LOSS_CHECK_USER_DESC,
    
    CUST1_IDX_BBPG_BREAK_CHECK_CHAR,
    CUST1_IDX_BBPG_BREAK_CHECK_VAL,
    CUST1_IDX_BBPG_BREAK_CHECK_NTF_CFG,
    CUST1_IDX_BBPG_BREAK_CHECK_USER_DESC,    

    CUST1_IDX_NB
};

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

extern struct attm_desc_128 custs1_att_db[CUST1_IDX_NB];

/// @} USER_CONFIG

#endif // _USER_CUSTS1_DEF_H_
