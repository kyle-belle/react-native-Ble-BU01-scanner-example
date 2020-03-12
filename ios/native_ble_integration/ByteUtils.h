//
//  ByteUtils.h
//  Ble
//
//  Created by xminnov on 18/8/27.
//  Copyright © 2018年 xminnov. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ByteUtils : NSObject

/**
 *  字节数据转十六进制字符串显示
 *
 *  @param data       字节数据
 *  @param spaceBytes 插入空格的字节间隔（0表示不插入空格）
 *
 *  @return 转换之后的十六进制字符串
 */
+ (NSString *)toHexString:(NSData *)data spaceBytes:(NSUInteger)spaceBytes;

/**
 *  十六进制字符串转字节数据
 *
 *  @param str 字符串
 *
 *  @return 转换后的字节数据
 */
+ (NSData *)toHexBytes:(NSString *)str;

//+ (UInt8)calcCrc:(const UInt8 *)bytes index:(UInt8)index count:(UInt8)count;

+ (NSNumber *)toDecNumber:(NSString *)str;

+ (NSNumber *)toHexNumber:(NSString *)str;

@end
