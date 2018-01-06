#ifndef RAM_H
#define RAM_H

/* 
  OPTION RAM
  The following are part of "OPTION RAM _opt_" where 
  _opt_ is either a "RamModel" (<256) or a length in 
  bytes (>=1024). This enables flexible use of PEEK 
  and POKE, IN and OUT, from the command line app, 
  without requiring "dedicated hardware" (which comes 
  with restrictions).
*/

static char mRAMaddr[65536];

struct Ram
{
  char strict;            /* I/O restrictions, and platform specific commands */
  unsigned int size;      /* TotalRAM */
  unsigned int model;     /* RamModel */
  unsigned int swModel;   /* bank/page swicth model */
  unsigned int *bank;     /* bank map for Bank Switching */
  unsigned int *page;     /* page map for Page Switching */
  unsigned int *addr;     /* current address for Program Counter (emu) */
  unsigned int *io;       /* io map for IN/OUT and PEEK/POKE restrictions */
}

/*
  RAM models: For Retro machines, the actual structure 
  is the 64Kb address space when a machine is freshly 
  powered on, and the cursor is at the built-in BASIC 
  prompt. This technically requires an I/O map to be 
  accurate, as some addresses can only be PEEK'ed, not 
  POKE'd, while others can only be IN'ed or OUT'ed
*/

enum RamModel
{
  RM_ZERO=0,       /* 64Kb zero'd */
  RM_ONED,         /* 64Kb 255'd  */
  RM_ZX80,         /*  8Kb Sinclair ZX80 */
  RM_ZX81,         /*  8Kb Sinclair ZX81 */
  RM_ZX81_16,      /* 16Kb Sinclair ZX81 */
  RM_ZX16,         /* 16Kb Sinclair ZX Spectrum */
  RM_ZX64,         /* 64Kb Sinclair ZX Spectrum */
  RM_VIC20,        /* Commodore Vic 20   */
  RM_C64,          /* Commodore C64      */
  RM_C16,          /* Commodore C16      */
  RM_CP4,          /* Commodore Plus 4   */
  RM_CPM14,        /*  64Kb CP/M 1.4 */
  RM_CPM20,        /*  64Kb CP/M 2.0 */
  RM_CPM22,        /*  64Kb CP/M 2.2 */
  RM_MSX22,        /*  64Kb MSX-DOS 1 (CP/M 2.2) */
  RM_CPM30,        /*  64Kb CPM 3.0 */
  RM_CPM30_128,    /* 128Kb CPM 3.0 */
  RM_CPM31,        /*  64Kb CPM 3.1 */
  RM_CPM31_128,    /* 128Kb CPM 3.1 */
  RM_CPC464,       /*  64Kb Amstrad CPC 464 (cassette) */
  RM_CPC664,       /*  64Kb Amstrad CPC 664 (disc)     */
  RM_CPC6128,      /* 128Kb Amstrad CPC 6128 (disc)    */
  RM_PCW8256,      /* 256Kb Amstrad PCW 8256 (1 disc)  */
  RM_PCW8512,      /* 512Kb Amstrad PCW 8512 (2 disc)  */
  RM_PCW9256,      /* 256Kb Amstrad PCW 9256 (1 disk)  */
  RM_PCW9512,      /* 512Kb Amstrad PCW 9512 (2 disk)  */
  RM_MSX,          /*  64Kb MSX 1    */
  RM_MSX_16,       /*  16Kb MSX 1    */
  RM_MSX_32,       /*  32Kb MSX 1    */
  RM_MSX2,         /*  64Kb MSX 2    */
  RM_MSX2_16,      /*  16Kb MSX 2    */
  RM_MSX2_32,      /*  32Kb MSX 2    */
  RM_MSX2_128,     /* 128Kb MSX 2    */
  RM_MSX2_256,     /* 256Kb MSX 2    */
  RM_MSX2_512,     /* 512Kb MSX 2    */
  RM_MSX2_1M,      /*   1Mb MSX 2    */
  RM_MSX2_2M,      /*   2Mb MSX 2    */
  RM_MSX2_4M,      /*   4Mb MSX 2    */
  RM_MSX2P,        /*  64Kb MSX 2+   */
  RM_MSX2P_16,     /*  16Kb MSX 2+   */
  RM_MSX2P_32,     /*  32Kb MSX 2+   */
  RM_MSX2P_128,    /* 128Kb MSX 2+   */
  RM_MSX2P_256,    /* 256Kb MSX 2+   */
  RM_MSX2P_512,    /* 512Kb MSX 2+   */
  RM_MSX2P_1M,     /*   1Mb MSX 2+   */
  RM_MSX2P_2M,     /*   2Mb MSX 2+   */
  RM_MSX2P_4M,     /*   4Mb MSX 2+   */
  RM_APPLE,        /*   4Kb Apple      */
  RM_APPLE_16,     /*  16Kb Apple      */
  RM_APPLE2,       /*  48Kb Apple ][   */
  RM_APPLE2P,      /*  48Kb Apple ][+  */
  RM_APPLE2E,      /*  64Kb Apple //e  */
  RM_APPLE2EN,     /* 128Kb Apple //e Enhanced */
  RM_APPLE2C,      /* 128Kb Apple //c  */
  RM_APPLE2C_1M,   /*   1Mb Apple //c  */
  RM_APPLE2C,      /* 128Kb Apple //c+ */
  RM_APPLE2C_1M,   /*   1Mb Apple //c+ */
  RM_FORMOSA,      /*  48Kb Formosa, Taiwanese Apple ][+ compatible  */
  RM_BASIS,        /* 128Kb BASIS 108, German Apple ][+ compatible, Z80 coprocessor */
  RM_CAT,          /*  64Kb DSE CAT (Laser 3000), Australian (Hong Kong) Apple //e compatible */
  RM_LAZER,        /* 128Kb Laser 128EX/2, Apple //e Enhanced compatible, //c+ predecessor */
  RM_LAZER_1M,     /*   1Mb Laser 128EX/2, Apple //e Enhanced compatible, //c+ predecessor */
  RM_ATARI,
  RM_LAST=RM_ATARI
};

#endif
