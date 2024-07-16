/*
 * Copyright (c) 2018 naehrwert
 * Copyright (c) 2018-2024 CTCaer
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#include <utils/types.h>
#include <mem/minerva.h>

#define NYX_NEW_INFO 0x3058594E

typedef enum
{
	REBOOT_RCM,          // PMC reset. Enter RCM mode.
	REBOOT_BYPASS_FUSES, // PMC reset via watchdog. Enter Normal mode. Bypass fuse programming in package1.

	POWER_OFF,           // Power off PMIC. Do not reset regulators.
	POWER_OFF_RESET,     // Power off PMIC. Reset regulators.
	POWER_OFF_REBOOT,    // Power off PMIC. Reset regulators. Power on.
} power_state_t;

typedef enum
{
	NYX_CFG_UMS  = BIT(6),

	NYX_CFG_EXTRA = 0xFF << 24
} nyx_cfg_t;

typedef enum
{
	ERR_LIBSYS_LP0 = BIT(0),
	ERR_SYSOLD_NYX = BIT(1),
	ERR_LIBSYS_MTC = BIT(2),
	ERR_SD_BOOT_EN = BIT(3),
	ERR_PANIC_CODE = BIT(4),
	ERR_L4T_KERNEL = BIT(24),
	ERR_EXCEPTION  = BIT(31),
} hekate_errors_t;

typedef struct _reg_cfg_t
{
	u32 idx;
	u32 val;
} reg_cfg_t;

typedef struct _nyx_info_t
{
	u32 magic;
	u32 sd_init;
	u32 sd_errors[3];
	u8  rsvd[0x1000];
	u32 disp_id;
	u32 errors;
} nyx_info_t;

typedef struct _nyx_storage_t
{
	u32 version;
	u32 cfg;
	u8  irama[0x8000];
	u8  hekate[0x30000];
	u8  rsvd[SZ_8M - sizeof(nyx_info_t)];
	nyx_info_t info;
	mtc_config_t mtc_cfg;
	emc_table_t mtc_table[11]; // 10 + 1.
} nyx_storage_t;

// TMR registers.
#define TIMERUS_CNTR_1US   (0x10 + 0x0)
#define TIMERUS_USEC_CFG   (0x10 + 0x4)
#define TIMER_TMR8_TMR_PTV 0x78
#define TIMER_TMR9_TMR_PTV 0x80
#define  TIMER_PER_EN       BIT(30)
#define  TIMER_EN           BIT(31)
#define TIMER_TMR8_TMR_PCR 0x7C
#define TIMER_TMR9_TMR_PCR 0x8C
#define  TIMER_INTR_CLR     BIT(30)

// WDT registers.
#define TIMER_WDT4_CONFIG         (0x100 + 0x80)
#define  TIMER_SRC(TMR)    ((TMR) & 0xF)
#define  TIMER_PER(PER)    (((PER) & 0xFF) << 4)
#define  TIMER_IRQENABL_EN BIT(12)
#define  TIMER_FIQENABL_EN BIT(13)
#define  TIMER_SYSRESET_EN BIT(14)
#define  TIMER_PMCRESET_EN BIT(15)
#define TIMER_WDT4_COMMAND        (0x108 + 0x80)
#define  TIMER_START_CNT   BIT(0)
#define  TIMER_CNT_DISABLE BIT(1)
#define TIMER_WDT4_UNLOCK_PATTERN (0x10C + 0x80)
#define  TIMER_MAGIC_PTRN  0xC45A

u8   bit_count(u32 val);
u32  bit_count_mask(u8 bits);
u64  sqrt64(u64 num);
long strtol(const char *nptr, char **endptr, register int base);
int  atoi(const char *nptr);

void reg_write_array(u32 *base, const reg_cfg_t *cfg, u32 num_cfg);
u16  crc16_calc(const u8 *buf, u32 len);
u32  crc32_calc(u32 crc, const u8 *buf, u32 len);

u32  get_tmr_us();
u32  get_tmr_ms();
u32  get_tmr_s();
void usleep(u32 us);
void msleep(u32 ms);

void panic(u32 val);
void power_set_state(power_state_t state);
void power_set_state_ex(void *param);


#endif
