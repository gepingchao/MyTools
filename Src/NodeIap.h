#ifndef __NODE_IAP_H__
#define __NODE_IAP_H__

#include "include.h"


#define SOF_CODE	0X01
#define EOF_CODE	0X17
#define OTHER_CODE	0X07

/* Bootloader command definitions. */
#define Bootloader_1_COMMAND_CHECKSUM          (0x31u)    /* Verify the checksum for the bootloadable project   */
#define Bootloader_1_COMMAND_REPORT_SIZE       (0x32u)    /* Report the programmable portions of flash          */
#define Bootloader_1_COMMAND_APP_STATUS        (0x33u)    /* Gets status info about the provided app status     */
#define Bootloader_1_COMMAND_ERASE             (0x34u)    /* Erase the specified flash row                      */
#define Bootloader_1_COMMAND_SYNC              (0x35u)    /* Sync the bootloader and host application           */
#define Bootloader_1_COMMAND_APP_ACTIVE        (0x36u)    /* Sets the active application                        */
#define Bootloader_1_COMMAND_DATA              (0x37u)    /* Queue up a block of data for programming           */
#define Bootloader_1_COMMAND_ENTER             (0x38u)    /* Enter the bootloader                               */
#define Bootloader_1_COMMAND_PROGRAM           (0x39u)    /* Program the specified row                          */
#define Bootloader_1_COMMAND_GET_ROW_CHKSUM    (0x3Au)    /* Compute flash row checksum for verification        */
#define Bootloader_1_COMMAND_EXIT              (0x3Bu)    /* Exits the bootloader & resets the chip             */
#define Bootloader_1_COMMAND_GET_METADATA      (0x3Cu)    /* Reports the metadata for a selected application    */
#define Bootloader_1_COMMAND_VERIFY_FLS_ROW    (0x45u)    /* Verifies data in buffer with specified row in flash*/


unsigned short Bootloader_CalcPacketChecksum(unsigned char* buffer, unsigned short size);
void Iap_SendDataToNode(unsigned char CMD,unsigned char*data,unsigned short data_length);
void EnterIapModCMD(void);

void IapSendTest(void);
#endif

