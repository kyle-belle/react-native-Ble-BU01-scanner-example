//
//  BU01_BleReader.h
//  Ble.BU01
//
//  Created by xminnov on 18/8/30.
//  Copyright © 2018 xminnov. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 *  delegate of ble reader event
 */
@protocol BU01_BleReaderDelegate

/**
 *  event of ble connect succeeded
 */
- (void)didConnect;

/**
 *  event of ble connect failed
 */
- (void)didFailConnect;

/**
 *  event of ble disconnect
 */
- (void)didDisconnect;

@end

enum {
    
    /**
     *  operate successfully
     */
    STATUS_SUCCESS          = 0,
    
    /**
     *  invalid parameter
     */
    INVALID_PARAMETER       = -101,
    
    /**
     *  device is not opened
     */
    DEVICE_NOT_OPENED       = -201,
    
    /**
     *  device is busy
     */
    DEVICE_BUSY             = -202,
    
    /**
     *  device reply error
     */
    DEVICE_NOASK            = -203,
    
    /**
     *  device operate failed
     */
    DEVICE_FAILED           = -204,
    
    /**
     *  device low power
     */
    DEVICE_BATTERY_LOW      = -205,
    
    /**
     *  communication time out
     */
    COMMUNICATE_TIMEOUT     = -301,
    
    /**
     *  communication error
     */
    COMMUNICATE_ERROR       = -302,
    
    /**
     *  communication failed
     */
    COMMUNICATE_FAILED      = -303,
    
};

/**
 the data of tag from inventory
 */
@interface BU01_Tag : NSObject

/**
 PC value
 */
@property(readonly) NSData* pc;

/**
 EPC value
 */
@property(readonly) NSData* epc;

/**
 RSSI value
 */
@property(readonly) double rssi;

@end

/**
 callback of getting version of reader

 @param int success: 0; failed: reference error code
 @param NSString version of reader
 */
typedef void (^get_version_callback)(int, NSString *);

/**
 callback of getting band area of reader

 @param int success: 0; failed: reference error code
 @param UInt8 band area of reader 
                        --Korea   	(0x11)
                        --US     	(0x21)
                        --US2    	(0x22)
                        --Europe  	(0x31)
                        --Japan    	(0x41)
                        --China1   	(0x51)
                        --China2   	(0x52)

 */
typedef void (^get_area_callback)(int, UInt8);

/**
 callback of setting band area of reader

 @param int success: 0; failed: reference error code
 */
typedef void (^set_area_callback)(int);

/**
 callback of getting RF power of reader

 @param int success: 0; failed: reference error code
 @param UInt8 RF power of reader is range from 15 to 25
 */
typedef void (^get_power_callback)(int, UInt8);

/**
 callback of setting RF power of reader

 @param int success: 0; failed: reference error code
 */
typedef void (^set_power_callback)(int);

/**
 callback of inventorying tags once

 @param int success: 0; failed: reference error code
 @param tags array of BU01_Tag
 */
typedef void (^inventory_callback)(int, NSArray *tags);

/**
 callback of reading tag

 @param int success: 0; failed: reference error code
 @param data the result of reading
 */
typedef void (^read_callback)(int, NSData *data);

/**
 callback of writing data to tag

 @param int success: 0; failed: reference error code
 */
typedef void (^write_callback)(int);

/**
 callback of locking tag

 @param int success: 0; failed: reference error code
 */
typedef void (^lock_callback)(int);

/**
 callback of killing tag

 @param int success: 0; failed: reference error code
 */
typedef void (^kill_callback)(int);

/**
 callback of getting battery of reader

 @param int success: 0; failed: reference error code
 @param UInt8 battery of reader
 */
typedef void (^get_battery_callback)(int, UInt8);

/**
 callback of getting beep enabled status of reader

 @param int success: 0; failed: reference error code
 @param UInt8 the enabled status of beep, enable:0x01, disable:0x00
 */
typedef void (^get_beep_callback)(int, UInt8);

/**
 callback of setting beep enabled status of reader

 @param int success: 0; failed: reference error code
 */
typedef void (^set_beep_callback)(int);

/**
 callback of setting name of reader 

 @param int success: 0; failed: reference error code
 */
typedef void (^set_name_callback)(int);

/**
 callback of reading temperature from temperature tag

 @param int success: 0; failed: reference error code
 @param NSData original data of temperature
 */
typedef void (^read_tag_temperature_callback)(int, NSData*);

/**
 callback of getting channel of reader

 @param int success: 0; failed: reference error code
 @param UInt8 channel is related to band area
 */
typedef void (^get_channel_callback)(int, UInt8);

/**
 callback of setting channel of reader

 @param int success: 0; failed: reference error code
 */
typedef void (^set_channel_callback)(int);

/**
 callback of getting light indicator of reader

 @param int success: 0; failed: reference error code
 @param UInt8 the enabled status of left light,enable:0x01, disable:0x00
 @param UInt8 the enabled status of middle light,enable:0x01, disable:0x00
 @param UInt8 the enabled status of right light,enable:0x01, disable:0x00
 */
typedef void (^get_light_indicator_callback)(int, UInt8, UInt8, UInt8);

/**
 callback of setting light indicator of reader

 @param int success: 0; failed: reference error code
 */
typedef void (^set_light_indicator_callback)(int);

/**
 callback of getting serial number of reader

 @param int success: 0; failed: reference error code
 @param NSData dec serial number of reader, max length is 10
 */
typedef void (^get_serial_number_callback)(int, NSData*);

/**
 callback of setting serial number of reader

 @param int success: 0; failed: reference error code
 */
typedef void (^set_serial_number_callback)(int);


/**
 BU01_BleReader
 */
@interface BU01_BleReader : NSObject

/**
 delegate of BU01_BleReader
 */
@property(weak) id<BU01_BleReaderDelegate> delegate;

/**
 reader name from the bluetooth scan
 */
@property(readonly) NSString *name;

/**
 reader RSSI from the bluetooth scan
 */
@property(readonly) NSNumber *RSSI;

/**
 connect ble reader
 */
- (void)connect;

/**
 disconnect ble reader
 */
- (void)disconnect;

/**
 get version of reader

 @param callback get_version_callback
 */
- (void)getVersion:(get_version_callback)callback;

/**
 get band area of reader

 @param callback get_area_callback
 */
- (void)getArea:(get_area_callback)callback;

/**
 set band area of reader

 @param area band area of reader
							--Korea   	(0x11)
							--US     		(0x21)
							--US2    		(0x22)
							--Europe  	(0x31)
							--Japan    	(0x41)
							--China1   	(0x51)
							--China2   	(0x52)
 @param callback set_area_callback
 */
- (void)setArea:(UInt8)area callback:(set_area_callback)callback;

/**
 get RF power of reader

 @param callback get_power_callback
 */
- (void)getPower:(get_power_callback)callback;

/**
 set RF power of reader

 @param power RF power is range from 15 to 25
 @param callback set_power_callback
 */
- (void)setPower:(UInt8)power callback:(set_power_callback)callback;

/**
 inventory tags once

 @param callback inventory_callback
 */
- (void)inventory:(inventory_callback)callback;

/**
 read tag

 @param password the access pwd of tag, default pwd is 00000000
 @param epc the epc of tag to be read
 @param bank the bank of tag to be operated
 							--RFU   	(0x00)
							--EPC     (0x01)
							--TID    	(0x02)
							--USER  	(0x03)
 @param offset the start address in bank of tag
 @param length the length to operate
 @param callback read_callback
 */
- (void)read:(UInt32)password epc:(NSData *)epc bank:(UInt8)bank offset:(UInt16)offset length:(UInt16)length callback:(read_callback)callback;

/**
 write data to tag

 @param password the access pwd of tag, default pwd is 00000000
 @param epc the epc of tag to be written
 @param bank the bank of tag to be operated
	 						--RFU   	(0x00)
							--EPC     (0x01)
							--TID    	(0x02)
							--USER  	(0x03)
 @param offset the start address in bank of tag
 @param length the length to operate
 @param data the data to be written
 @param callback write_callback
 */
- (void)write:(UInt32)password epc:(NSData *)epc bank:(UInt8)bank offset:(UInt16)offset length:(UInt16)length data:(NSData *)data callback:(write_callback)callback;

/**
 lock tag,the access pwd must be modify if it is 00000000

 @param password the access pwd of tag, default pwd is 00000000
 @param epc the epc of tag to be locked
 @param region the bank of tag to be operated
	 							--EPC   	(0x00)
								--USER     (0x01)
								--Acc pwd    	(0x02)
								--Kill pwd  	(0x03)
 @param action lock action
 								--unlock   	(0x00)
								--lock     (0x01)
								--permanently lock (0x02)
 @param callback lock_callback
 */
- (void)lock:(UInt32)password epc:(NSData *)epc region:(UInt8)region action:(UInt8)action callback:(lock_callback)callback;

/**
 kill tag, the kill pwd must be modify if it is 00000000

 @param password the kill pwd of tag, default pwd is 00000000
 @param epc the epc of tag to be killed
 @param callback kill_callback
 */
- (void)kill:(UInt32)password epc:(NSData *)epc callback:(kill_callback)callback;

/**
 get battery of reader

 @param callback get_battery_callback
 */
- (void)getBattery:(get_battery_callback)callback;

/**
 get beep enabled status of reader

 @param callback get_beep_callback
 */
- (void)getBeep:(get_beep_callback)callback;

/**
 set beep enabled status of reader

 @param status the beep status to be set,enable:0x01, disable:0x00
 @param callback set_beep_callback
 */
- (void)setBeep:(UInt8)status callback:(set_beep_callback)callback;
	
/**
 set name of reader

 @param name acsii format
 @param callback set_name_callback
 */
- (void)setName:(NSString *)name callback:(set_name_callback)callback;

/**
 read temperature from temperature tag

 @param password the access pwd of tag, default pwd is 00000000
 @param epc the epc of tag to be read
 @param callback read_tag_temperature_callback
 */
- (void)readTagTemperature:(UInt32)password epc:(NSData *)epc callback:(read_tag_temperature_callback)callback;

/**
 set channel of reader

 @param channel RF channel is related to band area
 @param callback set_channel_callback
 */
- (void)setChannel:(UInt8)channel callback:(set_channel_callback)callback;

/**
 get channel of reader

 @param callback get_channel_callback
 */
- (void)getChannel:(get_channel_callback)callback;

/**
 set light indicators of reader

 @param left left light,enable:0x01, disable:0x00
 @param middle middle light,enable:0x01, disable:0x00
 @param right right light,enable:0x01, disable:0x00
 @param callback set_light_indicator_callback
 */
- (void)setLightIndicator:(UInt8)left middle:(UInt8)middle right:(UInt8)right callback:(set_light_indicator_callback)callback;

/**
 get light indicators of reader

 @param callback get_light_indicator_callback
 */
- (void)getLightIndicator:(get_light_indicator_callback)callback;

/**
 set serial number of reader

 @param serialNumber serial number is dec and max length is 10
 @param callback set_serial_number_callback
 */
- (void)setSerialNumber:(NSData *)serialNumber callback:(set_serial_number_callback)callback;

/**
 get serial number of reader

 @param callback get_serial_number_callback
 */
- (void)getSerialNumber:(get_serial_number_callback)callback;


@end
