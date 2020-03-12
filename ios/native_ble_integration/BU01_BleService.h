//
//  BU01_BleService.h
//  Ble.BU01
//
//  Created by xminnov on 18/8/30.
//  Copyright © 2018 xminnov. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BU01_BleReader.h"

/**
	the delegate of bluetooth service event
 */
@protocol BU01_BleServiceDelegate

/**
	event of ble service state update
	
 @param state ble service state
 					CBManagerStateUnknown = 0,
					CBManagerStateResetting = 1,
					CBManagerStateUnsupported = 2,
					CBManagerStateUnauthorized = 3,
					CBManagerStatePoweredOff = 4,
					CBManagerStatePoweredOn = 5,

 */
- (void)didUpdateState:(NSInteger)state;

/**
	event of ble device discovered
	
 @param reader discovered reader
 */
- (void)didDiscoverBleReader:(BU01_BleReader *)reader;

@end

/**
	interface of bluetooth service
 */
@interface BU01_BleService : NSObject

/**
	delegate of bluetooth service
 */
@property(weak) id<BU01_BleServiceDelegate> delegate;

/**
	ble scan start
 */
- (void)scanForBleReader;

/**
	ble scan stop
 */
- (void)stopScan;

@end
