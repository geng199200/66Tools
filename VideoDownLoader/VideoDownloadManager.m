//
//  VideoDownloadManager.m
//  66
//
//  Created by Jesse on 16/4/14.
//  Copyright © 2016年 66. All rights reserved.
//

#import "VideoDownloadManager.h"
#import "VideoDownloader.h"

@interface VideoDownloadManager ()
@property (nonatomic, strong) NSOperationQueue *operationQueue;
@end

@implementation VideoDownloadManager
@dynamic downloadCount;
@dynamic currentDownloadsCount;
#pragma mark - Init


- (instancetype)init
{
    self = [super init];
    if (self) {
        self.operationQueue = [[NSOperationQueue alloc] init];
        self.defaultDownloadPath = [NSString stringWithString:NSTemporaryDirectory()];
    }
    return self;
}

+ (instancetype)sharedInstance
{
    static dispatch_once_t onceToken;
    static id sharedManager = nil;
    dispatch_once(&onceToken, ^{
        sharedManager = [[[self class] alloc] init];
        [sharedManager setOperationQueueName:@"VideoDownloadManager_SharedInstance_Queue"];
    });
    return sharedManager;
}


#pragma mark - VideoDownloader Management


- (VideoDownloader *)startDownloadWithURL:(NSURL *)url
                                customPath:(NSString *)customPathOrNil
                                  delegate:(id<VideoDownloaderDelegate>)delegateOrNil
{
    NSString *downloadPath = customPathOrNil ? customPathOrNil : self.defaultDownloadPath;
    
    VideoDownloader *downloader = [[VideoDownloader alloc] initWithURL:url
                                                            downloadPath:downloadPath
                                                                delegate:delegateOrNil];
    [self.operationQueue addOperation:downloader];
    
    return downloader;
}

- (VideoDownloader *)startDownloadWithURL:(NSURL *)url
                                customPath:(NSString *)customPathOrNil
                             firstResponse:(void (^)(NSURLResponse *response))firstResponseBlock
                                  progress:(void (^)(uint64_t receivedLength, uint64_t totalLength, NSInteger remainingTime, float progress))progressBlock
                                     error:(void (^)(NSError *error))errorBlock
                                  complete:(void (^)(BOOL downloadFinished, NSString *pathToFile))completeBlock
{
    NSString *downloadPath = customPathOrNil ? customPathOrNil : self.defaultDownloadPath;
    
    VideoDownloader *downloader = [[VideoDownloader alloc] initWithURL:url
                                                            downloadPath:downloadPath
                                                           firstResponse:firstResponseBlock
                                                                progress:progressBlock
                                                                   error:errorBlock
                                                                complete:completeBlock];
    [self.operationQueue addOperation:downloader];
    
    return downloader;
}

- (void)startDownload:(VideoDownloader *)download
{
    [self.operationQueue addOperation:download];
}

- (void)cancelAllDownloadsAndRemoveFiles:(BOOL)remove
{
    for (VideoDownloader *blob in [self.operationQueue operations]) {
        [blob cancelDownloadAndRemoveFile:remove];
    }
}


#pragma mark - Custom Setters


- (void)setOperationQueueName:(NSString *)name
{
    [self.operationQueue setName:name];
}

- (BOOL)setDefaultDownloadPath:(NSString *)pathToDL error:(NSError *__autoreleasing *)error
{
    if ([[NSFileManager defaultManager] createDirectoryAtPath:pathToDL
                                  withIntermediateDirectories:YES
                                                   attributes:nil
                                                        error:error]) {
        _defaultDownloadPath = pathToDL;
        return YES;
    } else {
        return NO;
    }
}

- (void)setMaxConcurrentDownloads:(NSInteger)maxConcurrent
{
    [self.operationQueue setMaxConcurrentOperationCount:maxConcurrent];
}


#pragma mark - Custom Getters


- (NSUInteger)downloadCount
{
    return [self.operationQueue operationCount];
}

- (NSUInteger)currentDownloadsCount
{
    NSUInteger count = 0;
    for (VideoDownloader *blob in [self.operationQueue operations]) {
        if (blob.state == VideoDownloadStateDownloading) {
            count++;
        }
    }
    
    return count;
}


@end
