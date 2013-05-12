//
//  MCOMessageView.m
//  testUI
//
//  Created by DINH Viêt Hoà on 1/19/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOMessageView.h"

#import "MCOCIDURLProtocol.h"

@interface MCOMessageView () <MCOHTMLRendererIMAPDelegate>

@end

@implementation MCOMessageView {
    UIWebView * _webView;
    NSString * _folder;
    MCOAbstractMessage * _message;
    __weak id <MCOMessageViewDelegate> _delegate;
    BOOL _prefetchIMAPImagesEnabled;
    BOOL _prefetchIMAPAttachmentsEnabled;
}

@synthesize folder = _folder;
@synthesize delegate = _delegate;
@synthesize prefetchIMAPImagesEnabled = _prefetchIMAPImagesEnabled;
@synthesize prefetchIMAPAttachmentsEnabled = _prefetchIMAPAttachmentsEnabled;

+ (void) initialize
{
    [MCOCIDURLProtocol registerProtocol];
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    
    if(self) {
        _webView = [[UIWebView alloc] initWithFrame:[self bounds]];
        [_webView setAutoresizingMask:(UIViewAutoresizingFlexibleHeight|UIViewAutoresizingFlexibleWidth)];
        [_webView setDelegate:self];
        
        [self addSubview:_webView];
    }
    
    return self;
}

- (void) dealloc
{
    _message = nil;
    _folder = nil;
    _webView = nil;
}

- (void) setMessage:(MCOAbstractMessage *)message
{
    _message = message;
    
    [_webView stopLoading];
    [self _refresh];
}

- (MCOAbstractMessage *) message
{
    return _message;
}

- (void) _refresh
{
    NSString * content;
    
    if (_message == nil) {
        content = nil;
    }
    else {
        if ([_message isKindOfClass:[MCOIMAPMessage class]]) {
            content = [(MCOIMAPMessage *) _message htmlRenderingWithFolder:_folder delegate:self];
        }
        else if ([_message isKindOfClass:[MCOMessageBuilder class]]) {
            content = [(MCOMessageBuilder *) _message htmlRenderingWithDelegate:self];
        }
        else if ([_message isKindOfClass:[MCOMessageParser class]]) {
            content = [(MCOMessageParser *) _message htmlRenderingWithDelegate:self];
        }
        else {
            content = nil;
            MCAssert(0);
        }
    }
	NSLog(@"%s",__PRETTY_FUNCTION__);
	
	if (content == nil) {
		[_webView loadHTMLString:@"" baseURL:/*[NSURL URLWithString:urlString]*/nil];
		return;
	}
	
	NSMutableString * html = [NSMutableString string];
	NSURL * jsURL = [[NSBundle mainBundle] URLForResource:@"MCOMessageViewScript" withExtension:@"js"];
	[html appendFormat:@"<html><head><script src=\"%@\"></script></head><body'>%@</body><iframe src='x-mailcore-msgviewloaded:' style='width: 0px; height: 0px; border: none;'></iframe></html>",
	 [jsURL absoluteString], content];
	[_webView loadHTMLString:html baseURL:nil];
}

- (void) _loadImages
{
	NSLog(@"%s",__PRETTY_FUNCTION__);
	NSString * result = [_webView stringByEvaluatingJavaScriptFromString:@"findCIDImageURL()"];
	NSData * data = [result dataUsingEncoding:NSUTF8StringEncoding];
	NSError *error = nil;
	NSArray * imagesURLStrings = [NSJSONSerialization JSONObjectWithData:data options:0 error:&error];
	
	for(NSString * urlString in imagesURLStrings) {
		MCOAbstractPart * part = nil;
		NSURL * url;
		
		url = [NSURL URLWithString:urlString];
		if ([MCOCIDURLProtocol isCID:url]) {
			part = [self _partForCIDURL:url];
		}
		else if ([MCOCIDURLProtocol isXMailcoreImage:url]) {
			NSString * specifier = [url resourceSpecifier];
			NSString * partUniqueID = specifier;
			part = [self _partForUniqueID:partUniqueID];
		}
		
		NSString * partUniqueID = [part uniqueID];
		NSData * data = [[self delegate] MCOMessageView:self dataForPartWithUniqueID:partUniqueID];
		if (data == NULL) {
			[[self delegate] MCOMessageView:self fetchDataForPartWithUniqueID:partUniqueID downloadedFinished:^(NSError * error) {
				NSData * downloadedData = [[self delegate] MCOMessageView:self dataForPartWithUniqueID:partUniqueID];
				NSData * previewData = [[self delegate] MCOMessageView:self previewForData:downloadedData isHTMLInlineImage:[MCOCIDURLProtocol isCID:url]];
				NSString *filename = [NSString stringWithFormat:@"%lu", (unsigned long)urlString.hash];
				NSURL *cacheURL = [self cacheJPEGImageData:previewData withFilename:filename];				
				
				NSString *replaceScript = [NSString stringWithFormat:@"replaceImageSrc(\"%@\", \"%@\")", urlString, cacheURL.absoluteString];
				[_webView stringByEvaluatingJavaScriptFromString:replaceScript];
				
				[MCOCIDURLProtocol partDownloadedMessage:_message partUniqueID:partUniqueID data:previewData];
			}];
		}
		[MCOCIDURLProtocol startLoadingWithMessage:_message
									  partUniqueID:partUniqueID
											  data:data
										   request:nil];
	}
}

- (NSURL *)cacheJPEGImageData:(NSData *)imageData withFilename:(NSString *)filename {
	NSString *path = [[NSTemporaryDirectory() stringByAppendingPathComponent:filename] stringByAppendingPathExtension:@"jpg"];
	[imageData writeToFile:path atomically:YES];	
	return [NSURL fileURLWithPath:path];
}

- (MCOAbstractPart *) _partForCIDURL:(NSURL *)url
{
    return [_message partForContentID:[url resourceSpecifier]];
}

- (MCOAbstractPart *) _partForUniqueID:(NSString *)partUniqueID
{
    return [_message partForUniqueID:partUniqueID];
}

- (NSData *) _dataForIMAPPart:(MCOIMAPPart *)part folder:(NSString *)folder
{
    NSData * data;
    NSString * partUniqueID = [part uniqueID];
    data = [[self delegate] MCOMessageView:self dataForPartWithUniqueID:partUniqueID];
    if (data == NULL) {
        [[self delegate] MCOMessageView:self fetchDataForPartWithUniqueID:partUniqueID downloadedFinished:^(NSError * error) {
            [self _refresh];
        }];
    }
    return data;
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
    
    NSURLRequest *responseRequest = [self webView:webView resource:nil willSendRequest:request redirectResponse:nil fromDataSource:nil];
    
    //NSLog(@"responseRequest:%@", responseRequest);

    if(responseRequest == request) {
        return YES;
    } else {
        [webView loadRequest:responseRequest];
        return NO;
    }
    
}

- (NSURLRequest *)webView:(UIWebView *)sender resource:(id)identifier willSendRequest:(NSURLRequest *)request redirectResponse:(NSURLResponse *)redirectResponse fromDataSource:(id)dataSource
{
    MCOAbstractPart * part = NULL;
    
    if ([[[request URL] scheme] isEqualToString:@"x-mailcore-msgviewloaded"]) {
        [self _loadImages];
        return request;
    }
    
    if ([MCOCIDURLProtocol isCID:[request URL]]) {
        part = [self _partForCIDURL:[request URL]];
    }
	else if ([MCOCIDURLProtocol isXMailcoreImage:[ request URL]]) {
        NSString * specifier = [[request URL] resourceSpecifier];
        NSString * partUniqueID = specifier;
        part = [self _partForUniqueID:partUniqueID];
    }
    
    if (part != NULL) {
        if ([_message isKindOfClass:[MCOIMAPMessage class]]) {
            NSMutableURLRequest * mutableRequest = [request mutableCopy];
            NSString * partUniqueID = [part uniqueID];
            NSData * data = [[self delegate] MCOMessageView:self dataForPartWithUniqueID:partUniqueID];
            if (data == NULL) {
                [[self delegate] MCOMessageView:self fetchDataForPartWithUniqueID:partUniqueID downloadedFinished:^(NSError * error) {
                    NSData * downloadedData = [[self delegate] MCOMessageView:self dataForPartWithUniqueID:partUniqueID];
                    NSData * previewData = [[self delegate] MCOMessageView:self previewForData:downloadedData isHTMLInlineImage:[MCOCIDURLProtocol isCID:[request URL]]];
                    [MCOCIDURLProtocol partDownloadedMessage:_message partUniqueID:partUniqueID data:previewData];
                }];
            }
            [MCOCIDURLProtocol startLoadingWithMessage:_message
                                          partUniqueID:partUniqueID
                                                  data:data
                                               request:mutableRequest];
            
            return mutableRequest;
        }
        else if ([_message isKindOfClass:[MCOMessageParser class]]) {
            NSMutableURLRequest * mutableRequest = [request mutableCopy];
            NSString * partUniqueID = [part uniqueID];
            NSData * data = [(MCOAttachment *) part data];
            NSData * previewData = [[self delegate] MCOMessageView:self previewForData:data isHTMLInlineImage:[MCOCIDURLProtocol isCID:[request URL]]];
            [MCOCIDURLProtocol startLoadingWithMessage:_message
                                          partUniqueID:partUniqueID
                                                  data:previewData
                                               request:mutableRequest];
            
            return mutableRequest;
        }
        else {
            return request;
        }
    }
    else {
        return request;
    }
}

- (BOOL) MCOAbstractMessage:(MCOAbstractMessage *)msg canPreviewPart:(MCOAbstractPart *)part
{
    static NSMutableSet * supportedImageMimeTypes = NULL;
    if (supportedImageMimeTypes == NULL) {
        supportedImageMimeTypes = [[NSMutableSet alloc] init];
        [supportedImageMimeTypes addObject:@"image/png"];
        [supportedImageMimeTypes addObject:@"image/gif"];
        [supportedImageMimeTypes addObject:@"image/jpg"];
        [supportedImageMimeTypes addObject:@"image/jpeg"];
    }
    static NSMutableSet * supportedImageExtension = NULL;
    if (supportedImageExtension == NULL) {
        supportedImageExtension = [[NSMutableSet alloc] init];
        [supportedImageExtension addObject:@"png"];
        [supportedImageExtension addObject:@"gif"];
        [supportedImageExtension addObject:@"jpg"];
        [supportedImageExtension addObject:@"jpeg"];
    }
    
    if ([supportedImageMimeTypes containsObject:[[part mimeType] lowercaseString]])
        return YES;
    
    NSString * ext = nil;
    if ([part filename] != nil) {
        if ([[part filename] pathExtension] != nil) {
            ext = [[[part filename] pathExtension] lowercaseString];
        }
    }
    if (ext != nil) {
        if ([supportedImageExtension containsObject:ext])
            return YES;
    }
    
    if (![[self delegate] respondsToSelector:@selector(MCOMessageView:canPreviewPart:)]) {
        return false;
    }
    return [[self delegate] MCOMessageView:self canPreviewPart:part];
}

- (NSDictionary *) MCOAbstractMessage:(MCOAbstractMessage *)msg templateValuesForHeader:(MCOMessageHeader *)header
{
    if (![[self delegate] respondsToSelector:@selector(MCOMessageView:templateValuesForHeader:)]) {
        return nil;
    }
    return [[self delegate] MCOMessageView:self templateValuesForHeader:header];
}

- (NSDictionary *) MCOAbstractMessage:(MCOAbstractMessage *)msg templateValuesForPart:(MCOAbstractPart *)part
{
    if (![[self delegate] respondsToSelector:@selector(MCOMessageView:templateValuesForPartWithUniqueID:)]) {
        return nil;
    }
    return [[self delegate] MCOMessageView:self templateValuesForPartWithUniqueID:[part uniqueID]];
}

- (NSString *) MCOAbstractMessage_templateForMainHeader:(MCOAbstractMessage *)msg
{
    if (![[self delegate] respondsToSelector:@selector(MCOMessageView_templateForMainHeader:)]) {
        return nil;
    }
    return [[self delegate] MCOMessageView_templateForMainHeader:self];
}

- (NSString *) MCOAbstractMessage_templateForImage:(MCOAbstractMessage *)msg
{
    NSString * templateString;
    if ([[self delegate] respondsToSelector:@selector(MCOMessageView_templateForImage:)]) {
        templateString = [[self delegate] MCOMessageView_templateForImage:self];
    }
    else {
        templateString = @"<img src=\"{{URL}}\"/>";
    }
    templateString = [NSString stringWithFormat:@"<div id=\"{{CONTENTID}}\">%@</div>", templateString];
    return templateString;
}

- (NSString *) MCOAbstractMessage_templateForAttachment:(MCOAbstractMessage *)msg
{
    if (![[self delegate] respondsToSelector:@selector(MCOMessageView_templateForAttachment:)]) {
        return NULL;
    }
    NSString * templateString = [[self delegate] MCOMessageView_templateForAttachment:self];
    templateString = [NSString stringWithFormat:@"<div id=\"{{CONTENTID}}\">%@</div>", templateString];
    return templateString;
}

- (NSString *) MCOAbstractMessage_templateForMessage:(MCOAbstractMessage *)msg
{
    if (![[self delegate] respondsToSelector:@selector(MCOMessageView_templateForMessage:)]) {
        return NULL;
    }
    return [[self delegate] MCOMessageView_templateForMessage:self];
}

- (NSString *) MCOAbstractMessage_templateForEmbeddedMessage:(MCOAbstractMessage *)msg
{
    if (![[self delegate] respondsToSelector:@selector(MCOMessageView_templateForEmbeddedMessage:)]) {
        return NULL;
    }
    return [[self delegate] MCOMessageView_templateForEmbeddedMessage:self];
}

- (NSString *) MCOAbstractMessage_templateForEmbeddedMessageHeader:(MCOAbstractMessage *)msg
{
    if (![[self delegate] respondsToSelector:@selector(MCOMessageView_templateForEmbeddedMessageHeader:)]) {
        return NULL;
    }
    return [[self delegate] MCOMessageView_templateForEmbeddedMessageHeader:self];
}

- (NSString *) MCOAbstractMessage_templateForAttachmentSeparator:(MCOAbstractMessage *)msg
{
    if (![[self delegate] respondsToSelector:@selector(MCOMessageView_templateForAttachmentSeparator:)]) {
        return NULL;
    }
    return [[self delegate] MCOMessageView_templateForAttachmentSeparator:self];
}

- (NSString *) MCOAbstractMessage:(MCOAbstractMessage *)msg filterHTMLForPart:(NSString *)html
{
    if (![[self delegate] respondsToSelector:@selector(MCOMessageView:filteredHTMLForPart:)]) {
        return html;
    }
    return [[self delegate] MCOMessageView:self filteredHTMLForPart:html];
}

- (NSString *) MCOAbstractMessage:(MCOAbstractMessage *)msg filterHTMLForMessage:(NSString *)html
{
    if (![[self delegate] respondsToSelector:@selector(MCOMessageView:filteredHTMLForMessage:)]) {
        return html;
    }
    return [[self delegate] MCOMessageView:self filteredHTMLForMessage:html];
}

- (NSData *) MCOAbstractMessage:(MCOAbstractMessage *)msg dataForIMAPPart:(MCOIMAPPart *)part folder:(NSString *)folder
{
    return [self _dataForIMAPPart:part folder:folder];
}

- (void) MCOAbstractMessage:(MCOAbstractMessage *)msg prefetchAttachmentIMAPPart:(MCOIMAPPart *)part folder:(NSString *)folder
{
    if (!_prefetchIMAPAttachmentsEnabled)
        return;
    
    NSString * partUniqueID = [part uniqueID];
    [[self delegate] MCOMessageView:self fetchDataForPartWithUniqueID:partUniqueID downloadedFinished:^(NSError * error) {
        // do nothing
    }];
}

- (void) MCOAbstractMessage:(MCOAbstractMessage *)msg prefetchImageIMAPPart:(MCOIMAPPart *)part folder:(NSString *)folder
{
    if (!_prefetchIMAPImagesEnabled)
        return;
    
    NSString * partUniqueID = [part uniqueID];
    [[self delegate] MCOMessageView:self fetchDataForPartWithUniqueID:partUniqueID downloadedFinished:^(NSError * error) {
        // do nothing
    }];
}

@end
