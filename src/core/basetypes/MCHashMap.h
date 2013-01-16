#ifndef __MAILCORE_MCHASHMAP_H_

#define __MAILCORE_MCHASHMAP_H_

#include <mailcore/MCObject.h>

namespace mailcore {

	class String;
	class Array;
	struct HashMapCell;
	typedef HashMapCell HashMapIter;

	class HashMap : public Object {
	private:
		unsigned int mAllocated;
		unsigned int mCount;
		void ** mCells;
		HashMapIter * iteratorBegin();
		HashMapIter * iteratorNext(HashMapIter * iter);
		void allocate(unsigned int size);
		void init();
	public:
		HashMap();
		HashMap(HashMap * o);
		virtual ~HashMap();
		
		static HashMap * hashMap();

		virtual String * description();
		virtual Object * copy();
		
		virtual unsigned int count();
		virtual void setObjectForKey(Object * key, Object * value);
		virtual void removeObjectForKey(Object * key);
		virtual Object * objectForKey(Object * key);
		virtual Array * allKeys();
		virtual Array * allValues();
		virtual void removeAllObjects();
	};

}

#endif
