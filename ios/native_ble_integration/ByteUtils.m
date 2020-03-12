//
//  ByteUtils.m
//  Ble
//
//  Created by xminnov on 18/8/27.
//  Copyright © 2018年 xminnov. All rights reserved.
//

#import "ByteUtils.h"

@implementation ByteUtils

+ (NSString *)toHexString:(NSData *)data spaceBytes:(NSUInteger)spaceBytes {
    if (data == nil) return @"";
    NSMutableString *str = [NSMutableString string];
    UInt8 *bytes = (UInt8*)data.bytes;
    for (int i=0; i<data.length; i++) {
        [str appendFormat:@"%02X", bytes[i]];
        if (spaceBytes > 0 && (i + 1) % spaceBytes == 0) {
            [str appendString:@" "];
        }
    }
    return str;
}

+ (BOOL)validateHex:(NSString *)str {
    for (int i=0; i<str.length; i++) {
        unichar c = [str characterAtIndex:i];
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) {
            return FALSE;
        }
    }
    return TRUE;
}

+ (BOOL)validateDec:(NSString *)str {
    for (int i=0; i<str.length; i++) {
        unichar c = [str characterAtIndex:i];
        if (!((c >= '0' && c <= '9'))) {
            return FALSE;
        }
    }
    return TRUE;
}

+ (NSData *)toHexBytes:(NSString *)str {
    NSString *formatStr = [str stringByReplacingOccurrencesOfString:@" " withString:@""];
    if (![self validateHex:formatStr]) return nil;
    if (formatStr.length % 2 != 0) {
        return nil;
    }
    NSMutableData *data = [NSMutableData data];
    for (int i=0; i<formatStr.length; i+=2) {
        NSString *subStr = [formatStr substringWithRange:NSMakeRange(i, 2)];
        NSScanner *scanner = [NSScanner scannerWithString:subStr];
        unsigned result = 0;
        [scanner scanHexInt:&result];
        [data appendBytes:&result length:1];
    }
    return data;
}

+ (NSNumber *)toDecNumber:(NSString *)str {
    if (str.length == 0) return nil;
    if (![self validateDec:str]) return nil;
    NSScanner *scanner = [NSScanner scannerWithString:str];
    NSInteger result;
    [scanner scanInteger:&result];
    return [NSNumber numberWithInteger:result];
}

+ (NSNumber *)toHexNumber:(NSString *)str {
    if (str.length == 0) return nil;
    if (![self validateHex:str]) return nil;
    NSScanner *scanner = [NSScanner scannerWithString:str];
    unsigned result;
    [scanner scanHexInt:&result];
    return [NSNumber numberWithUnsignedInt:result];
}

@end
