//
//  MCOMessageView.m
//  testUI
//
//  Created by DINH Viêt Hoà on 1/19/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOMessageView.h"

#import <WebKit/WebKit.h>

#import "MCOCIDURLProtocol.h"

@interface MCOMessageView () <MCOHTMLRendererIMAPDelegate, WebResourceLoadDelegate>

@end

@implementation MCOMessageView {
    WebView * _webView;
    NSString * _folder;
    MCOAbstractMessage * _message;
    id <MCOMessageViewDelegate> _delegate;
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

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    
    _webView = [[WebView alloc] initWithFrame:[self bounds]];
    [_webView setAutoresizingMask:NSViewHeightSizable | NSViewWidthSizable];
    [_webView setResourceLoadDelegate:self];
    [self addSubview:_webView];
    
    return self;
}

- (void) dealloc
{
    [_message release];
    [_folder release];
    [_webView release];
    
    [super dealloc];
}

- (void) setMessage:(MCOAbstractMessage *)message
{
    [_message release];
    _message = [message retain];
    
    [_webView stopLoading:nil];
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
    [[_webView mainFrame] loadHTMLString:content baseURL:nil];
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

- (NSURLRequest *)webView:(WebView *)sender resource:(id)identifier willSendRequest:(NSURLRequest *)request redirectResponse:(NSURLResponse *)redirectResponse fromDataSource:(id)dataSource
{
    MCOAbstractPart * part = NULL;
    
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
            
            return [mutableRequest autorelease];
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
            
            return [mutableRequest autorelease];
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
        return NO;
    }
    return [[self delegate] MCOMessageView:self canPreviewPart:part];
}

- (BOOL) MCOAbstractMessage:(MCOAbstractMessage *)msg shouldShowPart:(MCOAbstractPart *)part
{
    if (![[self delegate] respondsToSelector:@selector(MCOMessageView:shouldShowPart:)]) {
        return YES;
    }
    return [[self delegate] MCOMessageView:self shouldShowPart:part];
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

- (NSString *) MCOAbstractMessage:(MCOAbstractMessage *)msg templateForMainHeader:(MCOMessageHeader *)header
{
    if (![[self delegate] respondsToSelector:@selector(MCOMessageView:templateForMainHeader:)]) {
        return nil;
    }
    return [[self delegate] MCOMessageView:self templateForMainHeader:header];
}

- (NSString *) MCOAbstractMessage:(MCOAbstractMessage *)msg templateForImage:(MCOAbstractPart *)part
{
    NSString * templateString;
    if ([[self delegate] respondsToSelector:@selector(MCOMessageView:templateForImage:)]) {
        templateString = [[self delegate] MCOMessageView:self templateForImage:part];
    }
    else {
        templateString = @"<img src=\"{{URL}}\"/>";
    }
    templateString = [NSString stringWithFormat:@"<div id=\"{{CONTENTID}}\">%@</div>", templateString];
    return templateString;
}

- (NSString *) MCOAbstractMessage:(MCOAbstractMessage *)msg templateForAttachment:(MCOAbstractPart *)part
{
    if (![[self delegate] respondsToSelector:@selector(MCOMessageView:templateForAttachment:)]) {
        return NULL;
    }
    NSString * templateString = [[self delegate] MCOMessageView:self templateForAttachment:part];
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

- (NSString *) MCOAbstractMessage:(MCOAbstractMessage *)msg templateForEmbeddedMessage:(MCOAbstractMessagePart *)part
{
    if (![[self delegate] respondsToSelector:@selector(MCOMessageView:templateForEmbeddedMessage:)]) {
        return NULL;
    }
    return [[self delegate] MCOMessageView:self templateForEmbeddedMessage:part];
}

- (NSString *) MCOAbstractMessage:(MCOAbstractMessage *)msg templateForEmbeddedMessageHeader:(MCOMessageHeader *)header
{
    if (![[self delegate] respondsToSelector:@selector(MCOMessageView:templateForEmbeddedMessageHeader:)]) {
        return NULL;
    }
    return [[self delegate] MCOMessageView:self templateForEmbeddedMessageHeader:header];
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
