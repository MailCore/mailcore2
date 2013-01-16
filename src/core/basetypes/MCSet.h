#ifndef __MAILCORE_CSET_H_

#define __MAILCORE_CSET_H_

#include <mailcore/MCObject.h>

namespace mailcore {

	class String;
	class Array;
	class HashMap;

	class Set : public Object {
	private:
		HashMap * mHash;
		void init();
	public:
		Set();
		Set(Set * o);
		virtual ~Set();
		
		static Set * set();
		static Set * setWithArray(Array * objects);

		virtual String * description();
		virtual Object * copy();
		
		virtual unsigned int count();
		virtual void addObject(Object * obj);
		virtual void removeObject(Object * obj);
		virtual bool containsObject(Object * obj);
		virtual Object * member(Object * obj);
		
		virtual Array * allObjects();
		virtual void removeAllObjects();
		virtual void addObjectsFromArray(Array * objects);
	};

}

#endif
