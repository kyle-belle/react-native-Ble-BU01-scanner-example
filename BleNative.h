#import <React/RCTBridgeModule.h>
#import <React/RCTEventEmitter.h>
#import "BU01_BleService.h"

@interface BleNative : RCTEventEmitter <RCTBridgeModule, BU01_BleServiceDelegate, BU01_BleReaderDelegate>

@property BOOL inventoring;
@property BU01_BleService *service;
@property BU01_BleReader *reader;
@property NSMutableArray *readers;
@property NSMutableArray *tagRecords;
@property RCTResponseSenderBlock searchFinishCallback;
@end
