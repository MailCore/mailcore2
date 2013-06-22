#include "MCMainThread.h"

#import <Foundation/Foundation.h>

#include "MCAutoreleasePool.h"

using namespace mailcore;

@interface LEPPPMainThreadCaller : NSObject {
	void (* _function)(void *);
	void * _context;
}

@property (nonatomic, assign) void (* function)(void *);
@property (nonatomic, assign) void * context;

- (void) call;

@end

@implementation LEPPPMainThreadCaller

@synthesize function = _function;
@synthesize context = _context;

- (void) call
{
    AutoreleasePool * pool = new AutoreleasePool();
	_function(_context);
    pool->release();
}

@end

void mailcore::callOnMainThread(void (* function)(void *), void * context)
{
	LEPPPMainThreadCaller * caller;
	caller = [[LEPPPMainThreadCaller alloc] init];
	[caller setFunction:function];
	[caller setContext:context];
	[caller performSelectorOnMainThread:@selector(call) withObject:nil waitUntilDone:NO];
	[caller release];
}

void mailcore::callOnMainThreadAndWait(void (* function)(void *), void * context)
{
	LEPPPMainThreadCaller * caller;
	caller = [[LEPPPMainThreadCaller alloc] init];
	[caller setFunction:function];
	[caller setContext:context];
	[caller performSelectorOnMainThread:@selector(call) withObject:nil waitUntilDone:YES];
	[caller release];
}

void * mailcore::callAfterDelay(void (* function)(void *), void * context, double time)
{
	LEPPPMainThreadCaller * caller;
	caller = [[LEPPPMainThreadCaller alloc] init];
	[caller setFunction:function];
	[caller setContext:context];
	[caller performSelector:@selector(call) withObject:nil afterDelay:time];
    return [caller autorelease];
}

void mailcore::cancelDelayedCall(void * delayedCall)
{
	LEPPPMainThreadCaller * caller = (LEPPPMainThreadCaller *) delayedCall;
    [NSObject cancelPreviousPerformRequestsWithTarget:caller selector:@selector(call) object:nil];
}
