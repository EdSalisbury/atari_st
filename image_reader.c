#include <stdio.h>
#include <stdint.h>

#include "defs.h"

int main()
{
    FILE *img_file;
    img_file = fopen("Battletech.img", "rb");
    if (!img_file)
    {
        printf("Unable to open file!\n");
        return 1;
    }

    /* Read partition table */
    partition_table pt;

    fseek(img_file, partition_table_offset, SEEK_SET);
    fread(&pt, sizeof(partition_table), 1, img_file);
    

    uint32_t partition_size = __builtin_bswap32(pt.entries[0].partition_size) * 512;
    printf("Partition Size: %d bytes\n", partition_size);
    
    uint32_t partition_offset = __builtin_bswap32(pt.entries[0].partition_offset) * 512;
    printf("Partition Offset: %d (0x%04x)\n", partition_offset, partition_offset);
    
    uint32_t pos = ftell(img_file);
    printf("Current position: %d (0x%04x)\n", pos, pos);

    fseek(img_file, partition_offset, SEEK_SET);
    pos = ftell(img_file);
    printf("Current position: %d (0x%04x)\n", pos, pos);

    /* Read boot sector */
    boot_sector bs;
    fread(&bs, sizeof(boot_sector), 1, img_file);
    
    printf("BPS = %d\n", bs.bps);
    printf("SPC = %d\n", bs.spc);
    printf("RES = %d\n", bs.res);
    printf("NFATS = %d\n", bs.nfats);
    printf("NDIRS = %d\n", bs.ndirs);
    printf("NSECTS = %d\n", bs.nsects);
    printf("MEDIA = %#02x\n", bs.media);
    printf("SPF = %d\n", bs.spf);
    printf("Checksum = %#04x\n", bs.checksum);
    
    uint32_t first_fat = 0x802;
    uint32_t second_fat = first_fat + bs.spf * (bs.bps / 512);

    pos = ftell(img_file);
    printf("Current position: %d (0x%04x)\n", pos, pos);
    // uint32_t first_fat_offset = partition_offset + bs.res * (bs.bps / 512);
    printf("First FAT offset: %#06x\n", first_fat);
    
    // uint32_t second_fat_offset = first_fat_offset + spf * (bps / 512);
    printf("Second FAT offset: 0x%04x\n", second_fat);

    // uint32_t root_dir_offset = second_fat_offset + spf * (bps / 512);
    // printf("Root dir offset: 0x%04x\n", root_dir_offset);


    fclose(img_file);
    /* Cluster offset:
        file_start_sector = reserved_sectors 
            + (no of FAT * sectors per FAT) 
            + (max_root_entty * 32 / bytes per sector)
            + ((file_start_cluster - 2) * sectors_per_cluster)
    */
    
    /*
    Partition layout
        reserved_sector_count = ??
        fat_size = ??
        boot_sector (beginning of a logical partition) is given in the MBR
        fat1_offset = boot_sector + reserved_sector_count
        fat2_offset = fat1_offset + fat_size
        root_dir_offset = fat2_offset + fat_size
        data_cluster = root_dir_offset + size_root_dir (num_dirs * (32/512))
        data_region_size = cluster_count * sectors_per_cluster
    */
    return 0;
}