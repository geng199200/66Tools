//
//  VideoDownloader.h
//  66
//
//  Created by Jesse on 16/4/14.
//  Copyright © 2016年 66. All rights reserved.
//

#import <Foundation/Foundation.h>

@class VideoDownloader;
@protocol VideoDownloaderDelegate <NSObject>

@optional
- (void)download:(VideoDownloader *)blobDownload didReceiveFirstResponse:(NSURLResponse *)response;
- (void)download:(VideoDownloader *)blobDownload
  didReceiveData:(uint64_t)receivedLength
         onTotal:(uint64_t)totalLength
        progress:(float)progress;
- (void)download:(VideoDownloader *)blobDownload
didStopWithError:(NSError *)error;
- (void)download:(VideoDownloader *)blobDownload
didFinishWithSuccess:(BOOL)downloadFinished
          atPath:(NSString *)pathToFile;

@end

typedef NS_ENUM(NSUInteger, VideoDownloadError) {
    VideoDownloadErrorInvalidURL = 0,
    VideoDownloadErrorHTTPError,
    VideoDownloadErrorNotEnoughFreeDiskSpace
};

typedef NS_ENUM(NSUInteger, VideoDownloadState) {
    VideoDownloadStateReady = 0,
    VideoDownloadStateDownloading,
    VideoDownloadStateDone,
    VideoDownloadStateCancelled,
    VideoDownloadStateFailed
};
@interface VideoDownloader : NSOperation<NSURLConnectionDelegate>
@property (nonatomic, unsafe_unretained) id<VideoDownloaderDelegate> delegate;
@property (nonatomic, copy, readonly) NSString *pathToDownloadDirectory;
@property (nonatomic, copy, readonly, getter = pathToFile) NSString *pathToFile;
@property (nonatomic, copy, readonly) NSURL *downloadURL;
@property (nonatomic, strong, readonly) NSMutableURLRequest *fileRequest;
@property (nonatomic, copy, getter = fileName) NSString *fileName;
@property (nonatomic, assign, readonly) NSInteger speedRate;
@property (nonatomic, assign, readonly, getter = remainingTime) NSInteger remainingTime;
@property (nonatomic, assign, readonly, getter = progress) float progress;

@property (nonatomic, assign, readonly) VideoDownloadState state;

- (instancetype)initWithURL:(NSURL *)url
               downloadPath:(NSString *)pathToDL
                   delegate:(id<VideoDownloaderDelegate>)delegateOrNil;
- (instancetype)initWithURL:(NSURL *)url
               downloadPath:(NSString *)pathToDL
              firstResponse:(void (^)(NSURLResponse *response))firstResponseBlock
                   progress:(void (^)(uint64_t receivedLength, uint64_t totalLength, NSInteger remainingTime, float progress))progressBlock
                      error:(void (^)(NSError *error))errorBlock
                   complete:(void (^)(BOOL downloadFinished, NSString *pathToFile))completeBlock;
- (void)cancelDownloadAndRemoveFile:(BOOL)remove;

- (void)addDependentDownload:(VideoDownloader *)download;
@end
