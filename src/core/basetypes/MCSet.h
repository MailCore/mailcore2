#ifndef __MAILCORE_CSET_H_

#define __MAILCORE_CSET_H_

#include <MailCore/MCObject.h>

#ifdef __cplusplus

namespace mailcore {

	class String;
	class Array;
	class HashMap;

	class Set : public Object {
	public:
		Set();
		Set(Set * o);
		
		static Set * set();
		static Set * setWithArray(Array * objects);

		virtual unsigned int count();
		virtual void addObject(Object * obj);
		virtual void removeObject(Object * obj);
		virtual bool containsObject(Object * obj);
		virtual Object * member(Object * obj);
		
		virtual Array * allObjects();
		virtual void removeAllObjects();
		virtual void addObjectsFromArray(Array * objects);
        
    public: // subclass behavior
		virtual ~Set();
		virtual String * description();
		virtual Object * copy();
        
	private:
		HashMap * mHash;
		void init();
	};

}

#endif

#endif
