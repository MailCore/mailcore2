//
//  main.mm
//  iOS UI Test
//
//  Created by Jonathan Willing on 4/8/13.
//  Copyright (c) 2013 AppJon. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "AppDelegate.h"
#import "Application.h"
#import <MailCore/MailCore.h>

int main(int argc, char *argv[]) {
	mailcore::logEnabled = false;
	mailcore::AutoreleasePool *pool = new mailcore::AutoreleasePool();
	return UIApplicationMain(argc, argv, NSStringFromClass([Application class]), NSStringFromClass([AppDelegate class]));
	pool->release();
}
