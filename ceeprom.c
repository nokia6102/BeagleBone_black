#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#define EEPROM_PATH "/sys/bus/i2c/devices/1-0050/eeprom"
#define READ_EEPROM_SIZE 160
#define BOM_NAME_LENGTH 20
#define SERIAL_NO_LENGTH 20
#define HW_VERSION_LENGTH 20
#define ID1_LENGTH 20
#define ID2_LENGTH 20
#define ID3_LENGTH 20
#define ID4_LENGTH 20
#define ID5_LENGTH 20

typedef struct _inventory_info {
    unsigned char bom_name[BOM_NAME_LENGTH];
    unsigned char serial_no[SERIAL_NO_LENGTH];
    unsigned char hw_version[HW_VERSION_LENGTH];
    unsigned char id1[ID1_LENGTH];
    unsigned char id2[ID2_LENGTH];
    unsigned char id3[ID3_LENGTH];
    unsigned char id4[ID4_LENGTH];
    unsigned char id5[ID5_LENGTH];
} inventory_info_t;

typedef struct eeprom_struct {
    union {
        inventory_info_t inventory;
        unsigned char raw_data[READ_EEPROM_SIZE];
    } buf;
} eeprom_t;

typedef enum _entry_id {
    UNKNOWN, 
    BOM_NAME,
    SERIAL_NO,
    HW_VERSION, 
    ID1, 
    ID2, 
    ID3, 
    ID4, 
    ID5
} entry_id_t;

static int eeprom_fd = 0;

int open_eeprom(eeprom_t* ep)
{
    ssize_t read_len = 0;

    if ((eeprom_fd = open(EEPROM_PATH, O_RDWR)) == -1)
        return -1;

    if ((read_len = read(eeprom_fd, (void*)ep, READ_EEPROM_SIZE)) != READ_EEPROM_SIZE) {
        close_eeprom(eeprom_fd);
        return -1;
    }

    return 0;
}

int close_eeprom(int fd)
{
    return close(eeprom_fd);
}

void read_eeprom(entry_id_t eid)
{
    eeprom_t eeprom;
    unsigned char buf[64] = {0,};

    if ((open_eeprom(&eeprom))) {
        printf("open/read eeprom failed!\n");
        return;
    }

    switch (eid) {
        case BOM_NAME:
            memcpy(buf, eeprom.buf.inventory.bom_name, BOM_NAME_LENGTH);
            printf("BOM name: %s\n", buf);
            break;
        case SERIAL_NO:
            memcpy(buf, eeprom.buf.inventory.serial_no, SERIAL_NO_LENGTH);
            printf("Serial No.: %s\n", buf);
            break;
        case HW_VERSION:
            memcpy(buf, eeprom.buf.inventory.hw_version, HW_VERSION_LENGTH);
            printf("Hardware version: %s\n", buf);
            break;
        case ID1:
            memcpy(buf, eeprom.buf.inventory.id1, ID1_LENGTH);
            printf("ID1: %s\n", buf);
            break;
        case ID2:
            memcpy(buf, eeprom.buf.inventory.id2, ID2_LENGTH);
            printf("ID2: %s\n", buf);
            break;
        case ID3:
            memcpy(buf, eeprom.buf.inventory.id3, ID3_LENGTH);
            printf("ID3: %s\n", buf);
            break;
        case ID4:
            memcpy(buf, eeprom.buf.inventory.id4, ID4_LENGTH);
            printf("ID4: %s\n", buf);
            break;
        case ID5:
            memcpy(buf, eeprom.buf.inventory.id5, ID5_LENGTH);
            printf("ID5: %s\n", buf);
            break;
    }

    close_eeprom(eeprom_fd);
}

void write_eeprom(entry_id_t eid, const char* data)
{
    eeprom_t eeprom;
    ssize_t write_len = 0;

    if ((open_eeprom(&eeprom))) {
        printf("open/read eeprom failed!\n");
        return;
    }

    switch (eid) {
    
   
  
 





        case ID1:
            strncpy(eeprom.buf.inventory.id1, data, ID1_LENGTH);
            break;
        case ID2:
            strncpy(eeprom.buf.inventory.id2, data, ID2_LENGTH);
            break;
        case ID3:
            strncpy(eeprom.buf.inventory.id3, data, ID3_LENGTH);
            break;
        case ID4:
            strncpy(eeprom.buf.inventory.id4, data, ID4_LENGTH);
            break;
        case ID5:
            strncpy(eeprom.buf.inventory.id5, data, ID5_LENGTH);
            break;
    }

    if (lseek(eeprom_fd, 0, SEEK_SET)) {
        printf("eeprom seek failed!\n");
        return;
    }
    if ((write_len = write(eeprom_fd, (void*)&eeprom, READ_EEPROM_SIZE)) != READ_EEPROM_SIZE) {
        printf("write eeprom failed!\n");
        close_eeprom(eeprom_fd);
        return;
    }
    close_eeprom(eeprom_fd);
}

entry_id_t parse_entry(char* arg)
{
    int arg_len = strlen(arg);

    if (!strncasecmp(arg, "bom", arg_len))
        return BOM_NAME;
    else if (!strncasecmp(arg, "serial", arg_len))
        return SERIAL_NO;
    else if (!strncasecmp(arg, "hw", arg_len))
        return HW_VERSION;
    else if (!strncasecmp(arg, "id1", arg_len))
        return ID1;
    else if (!strncasecmp(arg, "id2", arg_len))
        return ID2;
    else if (!strncasecmp(arg, "id3", arg_len))
        return ID3;
    else if (!strncasecmp(arg, "id4", arg_len))
        return ID4;
    else if (!strncasecmp(arg, "id5", arg_len))
        return ID5;
    else
        return UNKNOWN;
}

int main(int argc, char** argv)
{
    entry_id_t eid;

    if ((eid = parse_entry(argv[1])) == UNKNOWN) {
        printf("invalid arguments!\n");
        return -1;
    }
    
    printf("Customized eeprom utitly\n");
    switch (argc) {
        case 2:
            read_eeprom(eid);
            break;
        case 3:
            write_eeprom(eid, argv[2]);
            break;
        default:
            printf("invalid arguments!\n");
            return -1;
    }

    return 0;
}
