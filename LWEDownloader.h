//
//  LWEDownloader.h
//  jFlash
//
//  Created by Mark Makdad on 5/27/10.
//  Copyright 2010 Long Weekend Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zlib.h>
#import "LWEFile.h"
#import "ASIHTTPRequest.h"
#import "ModalTaskViewController.h"

// For ZIP extension
#define CHUNK 16384

#define HTTP_CODE_200_FOUND 200

/** State machine for the downloader */
typedef enum _downloaderStates
{
  kDownloaderReady,                   //! Downloader ready to go
  kDownloaderCancelled,               //! Downloader cancelled
  kDownloaderInsufficientSpace,       //! Insufficient space on disk to store file
  kDownloaderRetrievingMetaData,      //! Retrieving data about to-be-downloaded package (headers)
  kDownloaderRetrievingData,          //! Retrieving actual data
  kDownloaderPaused,                  //! Data retrieval paused
  kDownloaderNetworkFail,             //! Network lost/timeout (no data within certain time period)
  kDownloaderDownloadComplete,        //! Download complete (no more data)
  kDownloaderDecompressing,           //! Unzipping downloaded file
  kDownloaderDecompressFail,          //! Downloaded, but unzip failed (need to delete download file bf trying again)
  kDownloaderInstallFail,             //! Downloaded, unzipped, but not installed properly
  kDownloaderSuccess                  //! Downloaded & verified
} downloaderStates;

/**
 * Strict protocol - if the installer delegate does not implement installPluginWithPath: , this should fail
 */
@protocol LWEDownloaderInstallerDelegate <NSObject>
@required
/** Returns YES if install was successful, NO on failure */
- (BOOL)installPluginWithPath:(NSString *)filename;
@end

/**
 * Generalized downloader that retrieves files via HTTP asynchronously and optionally unzips them
 */
@interface LWEDownloader : NSObject <ASIHTTPRequestDelegate, ModalTaskViewDelegate>

{
  NSURL *targetURL;                   //! HTTP address of the file to get
  NSInteger downloaderState;          //! Internal class state - where are we in the download process (cf _downloaderStates)
  NSString *targetFilename;           //! Full path filename where the downloaded data will be placed
  NSString *_compressedFilename;      //! Interim (full path) filename to be used if receiving compressed file - generated by the class
  
  //! "Private" variable holding request instance
  ASIHTTPRequest *_request;
  
  //! Delegate for installation methods
  id<LWEDownloaderInstallerDelegate> delegate;
  
  //! Status messages et al for observers
  NSString *taskMessage;
  NSString *statusMessage;
  NSInteger statusCode;
  float progress;
  int requestSize;
  
  BOOL _remoteFileIsGzipCompressed;   //! Is the remote URL a Gzip file?
  BOOL _unzipShouldCancel;            //! State variable to communicate w/ background unzip thread
}

// Psuedo private methods
- (BOOL) _updateInternalState:(NSInteger)nextState;
- (BOOL) _updateInternalState:(NSInteger)nextState withTaskMessage:(NSString*)taskMsg;

// Custom getter & setter for progress
- (float) progress;
- (void) setProgress:(float)progress;
- (void) setProgressFromBackgroundThread:(NSNumber*)tmpNum;

- (id) initWithTargetURL: (NSString *) target targetPath:(NSString*)targetFilename;

// Delegate methods for the ModalTaskViewController
- (BOOL) isFailureState;
- (int) getFailureState;
- (BOOL) isSuccessState;
- (void) startTask;
- (void) cancelTask;
- (void) resetTask;

// Unzip file we have just downloaded - designed for background thread
- (BOOL) _unzipDownloadedFile;

// Internal method to kick off delegate
- (void) _verifyDownload;

//! Method that can (and should) be delegated via  LWEDownloaderInstallerDelegate protocol
- (BOOL) installPluginWithPath:(NSString *)filename;

// ASIHTTPRequest delegate methods
- (void)requestReceivedResponseHeaders:(ASIHTTPRequest *)request;
- (void)requestFinished:(ASIHTTPRequest *) request;
- (void)requestFailed:(ASIHTTPRequest *) request;


@property (nonatomic, retain) NSURL *targetURL;
@property (nonatomic, retain) NSString *targetFilename;
@property (nonatomic, retain) NSString *taskMessage;
@property (nonatomic, retain) NSString *statusMessage;
@property (assign, nonatomic, readwrite) id delegate;
@property float progress;

@end
