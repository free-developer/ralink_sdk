/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2006, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attempt
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    foe_fdb.h

    Abstract:

    Revision History:
    Who         When            What
    --------    ----------      ----------------------------------------------
    Name        Date            Modification logs
    Steven Liu  2006-10-06      Initial version
*/

#ifndef _FOE_FDB_WANTED
#define _FOE_FDB_WANTED

#include "hwnat_ioctl.h"

/*
 * DEFINITIONS AND MACROS
 */
#define FOE_ENTRY_LIFE_TIME	5
#define FOE_THRESHOLD		1000
#define FOE_HASH_MASK		0x0001FFFF
#define FOE_HASH_WAY		2
#define FOE_1K_SIZ_MASK		0x000001FF
#define FOE_2K_SIZ_MASK		0x000003FF
#define FOE_4K_SIZ_MASK		0x000007FF
#define FOE_8K_SIZ_MASK		0x00000FFF
#define FOE_16K_SIZ_MASK	0x00001FFF

#if defined (CONFIG_RA_HW_NAT_TBL_1K)
#define FOE_4TB_SIZ		1024
#elif defined (CONFIG_RA_HW_NAT_TBL_2K)
#define FOE_4TB_SIZ		2048
#elif defined (CONFIG_RA_HW_NAT_TBL_4K)
#define FOE_4TB_SIZ		4096
#elif defined (CONFIG_RA_HW_NAT_TBL_8K)
#define FOE_4TB_SIZ		8192
#elif defined (CONFIG_RA_HW_NAT_TBL_16K)
#define FOE_4TB_SIZ		16384
#endif

#define FOE_ENTRY_SIZ		128  /* for ipv6 backward compatible */


#define IP_FORMAT(addr) \
	((unsigned char *)&addr)[3], \
        ((unsigned char *)&addr)[2], \
        ((unsigned char *)&addr)[1], \
        ((unsigned char *)&addr)[0]

/*
 * TYPEDEFS AND STRUCTURES
 */

enum FoeTblSize {
    FoeTblSize_1K,
    FoeTblSize_2K,
    FoeTblSize_4K,
    FoeTblSize_8K,
    FoeTblSize_16K
};

enum VlanAction {
    NO_ACT=0,
    MODIFY=1,
    INSERT=2,
    DELETE=3
};

enum FoeEntryState {
    INVALID=0,
    UNBIND=1,
    BIND=2,
    FIN=3
};

enum FoeTblTcpUdp {
    TCP=0,
    UDP=1
};

enum FoeTblEE {
	NOT_ENTRY_END=0,
	ENTRY_END_FP=1,
	ENTRY_END_FOE=2
};

enum FoeLinkType {
	LINK_TO_FOE=0,
	LINK_TO_FP=1
};

/* state = unbind & static */
struct us_info_blk1 {
    uint8_t resv;
    uint8_t sip:1; /* source ip */
    uint8_t dip:1; /* destination ip */
    uint8_t sport:1; /* source port */
    uint8_t dport:1; /* destination port */
    uint8_t sa:1; /* source mac */
    uint8_t da:1; /* destination mac */
    uint8_t resv1:2;
    uint8_t v1:2; /* vlan1 field */
    uint8_t v2:2; /* vlan2 field */
    uint8_t snap:2; /* SNAP field */
    uint8_t pppoe:2; /* PPPoE field */
    uint8_t ttl:1;  
    uint8_t resv2:2;
    uint8_t minf:1; /* more information in block 2 */
    uint8_t state:2; /* state of entry (0: invalid, 1:unbind, 2: bind, 3: fin) */
    uint8_t resv3:1;
    uint8_t sta:1; /* static entry */
};

/* state = unbind & dynamic */
struct ud_info_blk1 {
    uint32_t time_stamp:8;
    uint32_t pcnt:16; /* packet count */
    uint32_t resv:4;
    uint32_t state:2; 
    uint32_t t_u:1; /* 0: tcp, 1: udp */
    uint32_t sta:1; /* static entry */
};


/* state = bind & fin */
struct bf_info_blk1 {
    uint16_t time_stamp;
    uint16_t v1:2;
    uint16_t v2:2;
    uint16_t snap:2;
    uint16_t pppoe:2;
    uint16_t ttl:1;
    uint16_t ka:1; /* keep alive */
    uint16_t resv:2;
    uint16_t state:2;
    uint16_t t_u:1; /* 0: tcp, 1:udp */
    uint16_t sta:1; /* static entry */
};

struct _info_blk2 {
    uint16_t fd:1; /* force destination */
    uint16_t dp:3; /* destination port (0:cpu,1:GE1) */
    uint16_t fp:1; /* force new user priority */
    uint16_t up:3; /* new user priority */
    uint16_t mae:1;  /* MTR/AC enable */
    uint16_t m_a:1; /* 0:meter, 1:account */
    uint16_t post_mag:6; /* meter/account group */
    uint16_t act_dp:3; /* actual destination port for software nat */
    uint16_t resv:13; 
};

struct FoeEntry {
    union {
	struct us_info_blk1 usib1;
	struct ud_info_blk1 udib1;
	struct bf_info_blk1 bfib1;
	uint32_t info_blk1;
    };
    uint32_t	sip;
    uint32_t	dip;
    uint16_t	dport;
    uint16_t	sport;

    union {
	struct _info_blk2 iblk2;
	uint32_t info_blk2;
    };

    uint32_t	new_sip;
    uint32_t	new_dip;
    uint16_t	new_dport;
    uint16_t	new_sport;
    
    uint8_t	dmac_hi[2];
    uint16_t	vlan1;
    uint8_t	dmac_lo[4];
    uint8_t	smac_hi[2];
    uint16_t	pppoe_id;
    uint8_t	smac_lo[4];

    uint8_t	snap_ctrl[3];
    uint8_t	resv1; 
    uint16_t	vlan2;
    uint16_t	resv2;
    uint32_t	resv3;
    struct bf_info_blk1 tmp_buf;
};

struct FoePriKey {
    uint32_t sip;
    uint32_t dip;
    uint16_t sport;
    uint16_t dport;
    uint32_t is_udp:1;
};

int32_t FoeHashFun(struct FoePriKey *key,enum FoeEntryState TargetState);
uint32_t FoeInsEntry(struct FoeEntry *entry);
void FoeSetMacInfo(uint8_t *dst, uint8_t *src);
void FoeGetMacInfo(uint8_t *dst, uint8_t *src);
void FoeDumpEntry(uint32_t Index);
int FoeAddEntry(struct hwnat_tuple *opt);
int FoeDelEntry(struct hwnat_tuple *opt);
int FoeGetAllEntries(struct hwnat_args *opt);
int FoeBindEntry(struct hwnat_args *opt);
int FoeUnBindEntry(struct hwnat_args *opt);
int FoeDelEntryByNum(uint32_t entry_num);
void FoeTblClean(void);

#endif
