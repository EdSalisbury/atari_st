#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>


const uint16_t partition_table_offset = 0x1c6;

/* Specify packed structure since gcc assumes padded by default */
typedef struct __attribute__((__packed__)) partition_entry {
    /* 
    Status: indicate the status of the partition
        bit 0   when set partition exist,
        bit 1-6 reserved
        bit 7   when set partition bootable
        The BIOS will boot the first partition that has bit 7 set
    */
    uint8_t status;

    /*
    ID Name: a 3-bytes ASCII field that identifies the type of partition
        GEM for regular (< 32MB) partition
        BGM for big (≥ 32MB) partition
        XGM for extended partition
    */
    char id_name[3];

    /*
    Offset to the beginning of the partition from the beginning of the
        hard disk. Specified in number of physical (512 bytes) sectors
    */
    uint32_t partition_offset;

    /* Size of the partition in number of physical (512 bytes) sectors */
    uint32_t partition_size;
} partition_entry;

typedef struct partition_table {
    partition_entry entries[4];
} partition_table;

/* Specify packed structures since gcc assumes padded by default */
typedef struct __attribute__((__packed__)) boot_sector {
    /* 
    680x0 BRA.S instruction to the bootstrap code in this sector if the
        disk is executable, otherwise it is unused. 
    */
    uint16_t bra;

    /* Reserved for use as any necessary filler information. */
    char oem[6];

    /* The low 24-bits of this long represent a unique disk serial number. */
    char serial[3];

    /*
        This is an Intel format word (big-endian) which indicates the size of a
        logical sector in number of bytes.
    */
    uint16_t bps;

    /*
        This is a byte which indicates the number of sectors per cluster (must be 
        a power of 2). The only value supported by GEMDOS is 2.
    */
    uint8_t spc;

    /*
        This is an Intel format word which indicates the number of reserved
        logical sectors at the beginning of the logical drive, including the boot
        sector itself. This value is usually one.
    */
    uint16_t res;

    /*
        This is a byte indicating the number of File Allocation Table's (FAT's)
        stored on the logical drive. Usually the value is two.
    */
    uint8_t nfats;

    /*
        This is an Intel format word indicating the total number of file name
        entries that can be stored in the root directory of the logical drive.
    */
    uint16_t ndirs;

    /*
        This is an Intel format word indicating the total number of logical sectors
        on a logical drive including the reserved sectors.  
    */
    uint16_t nsects;

    /* 
        This byte is the media descriptor. For hard disks this value is set to $F8.
        It is not used by the ST BIOS. 
    */
    uint8_t media;

    /* 
        This is an Intel format word indicating the size occupied by each of the
        FATs in number of logical sectors
    */
    uint16_t spf;

    /* 
        This is an Intel format word indicating the number of sectors per track.
        Not applicable to Hard Disk.
    */
    uint16_t spt;

    /*
        This is an Intel format word indicating the number of heads on the
        media. Not applicable to Hard Disk.
    */
    uint16_t nheads;

    /* 
        This is an Intel format word indicating the number of hidden sectors. Not
        applicable to Hard Disk.
    */
    uint16_t nhid;

    /* Boot code if any */
    char os_boot_code[480];

    /* Checksum */
    uint16_t checksum;
} boot_sector;

/* Specify packed structures since gcc assumes padded by default */
typedef struct __attribute__((__packed__)) dir_entry {
    /* 
    DOS file name (padded with spaces). The first byte can have the following special values:
        0x00 Entry is available and no subsequent entry is in use
        0x05 Initial character is actually 0xE5.
        0x2E 'Dot' entry; either '.' or '..'
        0xE5 Entry has been previously erased and is available
    */
    char filename[8];
    
    /* DOS file extension (padded with spaces) */
    char ext[3];

    /* 
    File Attributes
        Bit Mask Description
        0   0x01 Read Only
        1   0x02 Hidden
        2   0x04 System
        3   0x08 Volume Label
        4   0x10 Subdirectory
        5   0x20 Archive
        6   0x40 Device (internal use only, never found on disk)
        7   0x80 Unused
        An attribute value of 0x0F is used to designate a long file name entry.
    */
    char attr;

    /* Reserved */
    char reserved;

    /* Create time, fine resolution: 10ms units, values from 0 to 199 */
    char create_time_fine;

    /*
    Create time. The hour, minute and second are encoded according to the following bitmap:
        Bits  Description
        15-11 Hours (0-23)
        10-5  Minutes (0-59)
        4-0   Seconds/2 (0-29)
        Note that the seconds is recorded only to a 2 second resolution. Finer resolution for file
        creation is in create_time_fine.
    */
    char create_time[2];

    /*
    Create date. The year, month and day are encoded according to the following bitmap:
        Bits Description
        15-9 Year (0 = 1980, 127 = 2107)
        8-5  Month (1 = January, 12 = December)
        4-0  Day (1 - 31)
    */
    char create_date[2];

    /*
    Last access date. The year, month and day are encoded according to the following bitmap:
        Bits Description
        15-9 Year (0 = 1980, 127 = 2107)
        8-5  Month (1 = January, 12 = December)
        4-0  Day (1 - 31)
    */
    char last_access_date[2];
    
    /* EA-Index in FAT12 and FAT16. High 2 bytes of first cluster number in FAT32 */
    char ea_index[2];

    /*
    Last modified time; The hour, minute and second are encoded according to the following bitmap:
        Bits  Description
        15-11 Hours (0-23)
        10-5  Minutes (0-59)
        4-0   Seconds/2 (0-29)
    */
    char last_modified_time[2];

    /*
    Last modified date. The year, month and day are encoded according to the following bitmap:
        Bits Description
        15-9 Year (0 = 1980, 127 = 2107)
        8-5  Month (1 = January, 12 = December)
        4-0  Day (1 - 31)
    */
    char last_modified_date[2];

    /*
    First cluster in FAT12 and FAT16. Low 2 bytes of first cluster in FAT32. Entries with the
        Volume Label flag, subdirectory ".." pointing to root, and empty files with size 0 should
        have first cluster 0.
    */
    char first_cluster[2];

    /* File size in bytes. Entries with the Volume Label or Subdirectory flag set should have a size of 0. */
    /* TODO: Determine endianness */
    char file_size[4];
} dir_entry;

#endif