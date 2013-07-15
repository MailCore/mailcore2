//
//  MCTTableViewCell.m
//  iOS UI Test
//
//  Created by Paul Young on 14/07/2013.
//  Copyright (c) 2013 AppJon. All rights reserved.
//

#import "MCTTableViewCell.h"

@implementation MCTTableViewCell

- (void)prepareForReuse
{
    [self.messageRenderingOperation cancel];
    self.detailTextLabel.text = @" ";
}

@end
