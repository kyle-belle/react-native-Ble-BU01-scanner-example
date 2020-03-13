#import "BleNative.h"
#import "React/RCTLog.h"
#import "ByteUtils.h"

@implementation BleNative
{
bool hasListeners;
}

-(void)startObserving{
  hasListeners = YES;
}

-(void)stopObserving{
  hasListeners = NO;
}

-(NSDictionary *)constantsToExport{
  return @{@"ModuleName": @"BleNative", @"ModuleDescription":@"Native Module to wrap native bluetooth rfid scanner functionality", @"MaxReaders":@20, @"ScanCodes":@{@0:@"success", @-101:@"INVALID_PARAMETER", @-201: @"DEVICE_NOT_OPENED", @-202: @"DEVICE_BUSY", @-203:@"DEVICE_NOASK", @-204:@"DEVICE_FAILED", @-205:@"DEVICE_BATTERY_LOW", @-301:@"COMMUNICATE_TIMEOUT", @-302:@"COMMUNICATE_ERROR", @-303:@"COMMUNICATE_FAILED"}};
}

+(BOOL) requiresMainQueueSetup{
  return YES;
}

-(NSArray<NSString*>*)supportedEvents{
  return @[@"Event", @"StartBleSearch", @"FinishBleSearch", @"StopBleSearch", @"BleDeviceDiscovered", @"BleServiceInit", @"ConnectionSuccess", @"ConnectionFail", @"ConnectionDisconnect", @"StartScanning", @"StopScanning", @"ErrorScanning", @"TagScanned"];
}

-(void) eventReceived: (NSString*) message{
  
  NSString* eventName = [[NSString alloc] initWithFormat: @"BleEvent: %@", message];
  [self sendEventWithName:@"Event" body:@{@"name": eventName}];
}

- (void)didUpdateState:(NSInteger)state {
  if (state == 5) {
    [self scanForBleReader];
  }
}

- (void)didConnect {
  if(hasListeners){
    [self sendEventWithName:@"ConnectionSuccess" body:@{@"msg": @"Successfully Connected!"}];
  }
}

- (void)didDisconnect {
  if(hasListeners){
    [self sendEventWithName:@"ConnectionDisconnect" body:@{@"msg": @"Device Disconnected"}];
  }
}

- (void)didFailConnect {
  if(hasListeners){
    [self sendEventWithName:@"ConnectionFail" body:@{@"msg": @"Failed to Connected!"}];
  }
}

- (void)didDiscoverBleReader:(BU01_BleReader *)reader {
  RCTLogInfo(@"didDiscoverBleReader");
  [self.readers addObject:reader];
  [self sendEventWithName:@"BleDeviceDiscovered" body: @{@"readerName": reader.name, @"rssi": reader.RSSI}];
}

- (void)scanForBleReader {
  
  [self.readers removeAllObjects];
  [self.service scanForBleReader];
  
  double delayInSeconds = 4.0;
  dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
  dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
    [self.service stopScan];
    NSArray* readers = [[NSArray alloc] initWithArray: self.readers];
    if(self.searchFinishCallback){
//      self.searchFinishCallback(@[[NSNull null], readers]);
    }
    [self sendEventWithName:@"FinishBleSearch" body:@{@"readers": readers}];
  });
}

- (void)addTag:(BU01_Tag *)tag {
  NSString *epc = [ByteUtils toHexString:tag.epc spaceBytes:0];
  int i=0;
  for (; i<self.tagRecords.count; i++) {
    BU01_Tag *tagRecord = self.tagRecords[i][@"tag"];
    NSString* current_ecp =[ByteUtils toHexString:tagRecord.epc spaceBytes:0];
    if ([current_ecp isEqualToString:epc]) {
      NSNumber* count = [NSNumber numberWithInt: (((NSNumber*)self.tagRecords[i][@"count"]).intValue + 1)];
      [self.tagRecords setObject:@{@"tag": tagRecord, @"count": count} atIndexedSubscript:i];
      RCTLogInfo(@"tag epc: %@, rssi: %f , count: %@", [ByteUtils toHexString:((BU01_Tag*)self.tagRecords[i][@"tag"]).epc spaceBytes:0], ((BU01_Tag*)self.tagRecords[i][@"tag"]).rssi, (NSNumber*)self.tagRecords[i][@"count"]);
      
      [self sendEventWithName:@"TagScanned" body:@{@"ScannedTag": @{@"epc": current_ecp, @"rssi": [NSNumber numberWithDouble: tagRecord.rssi], @"count": count}}];
      break;
    }
  }
  if (i == self.tagRecords.count) {
    [self.tagRecords addObject:@{@"tag": tag, @"count": @1}];
    [self sendEventWithName:@"TagScanned" body:@{@"ScannedTag": @{@"epc": epc, @"rssi": [NSNumber numberWithDouble: tag.rssi], @"count": @1}}];
    RCTLogInfo(@"Adding new tag!!");
  }
}

- (void)inventory {
  
  [self.reader inventory:^(int ret, NSArray *tags) {
    
    if (ret == 0) {
      for (BU01_Tag *tag in tags) {
        RCTLogInfo(@"tag epc: %@, rssi: %f", [ByteUtils toHexString:tag.epc spaceBytes:0], tag.rssi);
        [self addTag: tag];
      }
      
      if (self.inventoring) {
        [self inventory];
      } else {
        [self sendEventWithName:@"StopScanning" body:@{@"msg": @"scanning stopped!"}];
      }
    } else {
      if (ret != 0) {
        [self sendEventWithName:@"ErrorScanning" body:@{@"errorCode": [NSNumber numberWithInt: ret]}];
        NSLog(@"inventory failed: %d", ret);
      }
    }
  }];
}

RCT_EXPORT_MODULE()

RCT_EXPORT_METHOD(BleInit: (RCTResponseSenderBlock) callback)
{
  // TODO: Implement some actually useful functionality
  self.service = [BU01_BleService new];
  self.service.delegate = self;
  self.readers = [NSMutableArray arrayWithCapacity: 20];
  self.tagRecords = [NSMutableArray arrayWithCapacity: 20];
  callback(@[[NSNull null], @"Ble Service Initialized"]);
  if(hasListeners){
    [self sendEventWithName:@"BleServiceInit" body:@{@"msg":@"Ble Service Initialized"}];
  }
}

RCT_EXPORT_METHOD(addEvent: (NSString*)name location: (NSString*) location callback: (RCTResponseSenderBlock) callback)
{
    // TODO: Implement some actually useful functionality
  RCTLogInfo(@"Pretending to create an event %@ at %@", name, location);
  callback(@[[NSNull null], name, location]);
  if(hasListeners){
    [self eventReceived: location];
  }
}

RCT_EXPORT_METHOD(searchForBleReader:(RCTResponseSenderBlock) callback)
{
  [self scanForBleReader];
  self.searchFinishCallback = callback;
  if(hasListeners){
    [self sendEventWithName:@"StartBleSearch" body:@{@"msg": @"Search Started!"}];
  }
}

RCT_EXPORT_METHOD(connectToBleReader: (NSDictionary*) reader index: (nonnull NSNumber*) index callback: (RCTResponseSenderBlock) callback){
  self.reader = self.readers[index.intValue];
  
  self.reader.delegate = self;
  
  [self.reader connect];
  
  RCTLogInfo(@"reader *: %@, reader Name: %@, reader RSSI: %@, Array Index: %@", self.reader, self.reader.name, self.reader.RSSI, index);
  
  callback(@[[NSNull null], @"connect"]);
}

RCT_EXPORT_METHOD(disconnectFromBleReader: (RCTResponseSenderBlock) callback){
  [self.reader disconnect];
  
  callback(@[[NSNull null], @"disconnected"]);
}

RCT_EXPORT_METHOD(StartScanning: (RCTResponseSenderBlock) callback){
  self.inventoring = true;
  [self inventory];
  [self sendEventWithName:@"StartScanning" body:@{@"msg": @"Started Scanning!"}];
  callback(@[[NSNull null]]);
}

RCT_EXPORT_METHOD(StopScanning: (RCTResponseSenderBlock) callback){
  self.inventoring = false;
  callback(@[[NSNull null]]);
}


@end
