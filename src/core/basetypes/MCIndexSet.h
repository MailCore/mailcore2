//
//  MCIndexSet.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/4/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCINDEXSET_H_

#define __MAILCORE_MCINDEXSET_H_

#include <MailCore/MCObject.h>
#include <MailCore/MCRange.h>
#include <inttypes.h>

#ifdef __cplusplus

namespace mailcore {
    
	class IndexSet : public Object {
	public:
		IndexSet();
		IndexSet(IndexSet * o);
		
		static IndexSet * indexSet();
        static IndexSet * indexSetWithRange(Range range);
        static IndexSet * indexSetWithIndex(uint64_t idx);
        
		virtual unsigned int count();
		virtual void addIndex(uint64_t idx);
		virtual void removeIndex(uint64_t idx);
		virtual bool containsIndex(uint64_t idx);
        
		virtual void addRange(Range range);
        virtual void removeRange(Range range);
		virtual void intersectsRange(Range range);
        
        virtual Range * allRanges();
        virtual unsigned int rangesCount();
		virtual void removeAllIndexes();
        
    public: // subclass behavior
		virtual ~IndexSet();
		virtual String * description();
		virtual Object * copy();
		virtual HashMap * serializable();
        virtual void importSerializable(HashMap * serializable);
        
	private:
		Range * mRanges;
        unsigned int mCount;
        unsigned int mAllocated;
		void init();
        int rangeIndexForIndex(uint64_t idx);
        int rangeIndexForIndexWithBounds(uint64_t idx, unsigned int left, unsigned int right);
        void addRangeIndex(unsigned int rangeIndex);
        void removeRangeIndex(unsigned int rangeIndex, unsigned int count);
        int rightRangeIndexForIndex(uint64_t idx);
        int rightRangeIndexForIndexWithBounds(uint64_t idx, unsigned int left, unsigned int right);
        int leftRangeIndexForIndex(uint64_t idx);
        int leftRangeIndexForIndexWithBounds(uint64_t idx, unsigned int left, unsigned int right);
        void mergeRanges(unsigned int rangeIndex);
        void tryToMergeAdjacentRanges(unsigned int rangeIndex);
	};

}

#endif

#endif
