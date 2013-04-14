//
//  MCEnumerator.h
//  mailcore2
//
//  Created by Robert Widmann on 4/14/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCEnumerator__
#define __mailcore2__MCEnumerator__

#include <MailCore/MCObject.h>
#include <MailCore/MCArray.h>

#ifdef __cplusplus

namespace mailcore {

	class Enumerator : public Object {
	public:
		virtual Array * allObjects() = 0;
		virtual Object * nextObject() = 0;
	};
	
}

#endif

#endif /* defined(__mailcore2__MCEnumerator__) */
