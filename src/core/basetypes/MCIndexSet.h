//
//  MCIndexSet.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/4/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCIndexSet__
#define __mailcore2__MCIndexSet__

#include <mailcore/MCObject.h>
#include <mailcore/MCRange.h>
#include <inttypes.h>

#ifdef __cplusplus

namespace mailcore {
    
	class IndexSet : public Object {
	public:
		IndexSet();
		IndexSet(IndexSet * o);
		
		static IndexSet * indexSet();
        
		virtual unsigned int count();
		virtual void addIndex(uint64_t idx);
		virtual void removeIndex(uint64_t idx);
		virtual bool containsIndex(uint64_t idx);
		
        virtual Range * allRanges();
		virtual void removeAllIndexes();
        
    public: // subclass behavior
		virtual ~IndexSet();
		virtual String * description();
		virtual Object * copy();
        
	private:
		Range * mRanges;
        unsigned int mCount;
        unsigned int mAllocated;
		void init();
        int rangeIndexForIndex(uint64_t idx);
        int rangeIndexForIndexWithBounds(uint64_t idx, unsigned int left, unsigned int right);
        void addRangeIndex(unsigned int rangeIndex);
        void removeRangeIndex(unsigned int rangeIndex);
        int rightRangeIndexForIndex(uint64_t idx);
        int rightRangeIndexForIndexWithBounds(uint64_t idx, unsigned int left, unsigned int right);
	};

}

#endif

#endif /* defined(__mailcore2__MCIndexSet__) */
