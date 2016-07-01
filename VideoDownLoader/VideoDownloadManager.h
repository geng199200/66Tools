//
//  VideoDownloadManager.h
//  66
//
//  Created by Jesse on 16/4/14.
//  Copyright © 2016年 66. All rights reserved.
//

#import <Foundation/Foundation.h>
@class VideoDownloader;
@protocol VideoDownloaderDelegate;

@interface VideoDownloadManager : NSObject
@property (nonatomic, copy) NSString *defaultDownloadPath;


@property (nonatomic, assign) NSUInteger downloadCount;


@property (nonatomic, assign) NSUInteger currentDownloadsCount;


+ (instancetype)sharedInstance;

- (VideoDownloader *)startDownloadWithURL:(NSURL *)url
                                customPath:(NSString *)customPathOrNil
                                  delegate:(id<VideoDownloaderDelegate>)delegateOrNil;


- (VideoDownloader *)startDownloadWithURL:(NSURL *)url
                                customPath:(NSString *)customPathOrNil
                             firstResponse:(void (^)(NSURLResponse *response))firstResponseBlock
                                  progress:(void (^)(uint64_t receivedLength, uint64_t totalLength, NSInteger remainingTime, float progress))progressBlock
                                     error:(void (^)(NSError *error))errorBlock
                                  complete:(void (^)(BOOL downloadFinished, NSString *pathToFile))completeBlock;

- (void)startDownload:(VideoDownloader *)download;

- (void)setOperationQueueName:(NSString *)name;


- (BOOL)setDefaultDownloadPath:(NSString *)pathToDL error:(NSError *__autoreleasing *)error;

- (void)setMaxConcurrentDownloads:(NSInteger)max;


- (void)cancelAllDownloadsAndRemoveFiles:(BOOL)remove;

@end
