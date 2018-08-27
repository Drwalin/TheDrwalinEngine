
#ifndef SMART_PTR_H
#define SMART_PTR_H

#include <new>

template < class T >
class SmartPtr;

template < class T >
class SmartPtrData final
{
private:
	
	T * data;
	SmartPtr<T> * oneOfThem;
	
	friend SmartPtr<T>;
	
	inline void PureRemoveMember( SmartPtr<T>* member );
	
	inline static void PureDestroyGroup( SmartPtrData<T> * ptr );
	
public:
	
	inline void AddMember( SmartPtr<T>* member );
	
	inline void RemoveMember( SmartPtr<T>* member );
	
	inline void DestroyGroup();
	
	SmartPtrData( SmartPtr<T>* member ) ;
	
	~SmartPtrData();
};



template < class T >
class SmartPtr final
{
private:
	
	SmartPtrData<T> * data;
	SmartPtr<T> * next;
	SmartPtr<T> * prev;
	
	friend SmartPtrData<T>;
	
	inline void InitData();
	
public:
	
	int CountReferences() const;
	
	inline bool IsAlone() const;
	inline void JoinOtherToGroup( SmartPtr & other );
	
	inline const T* GetPtr() const;
	inline operator bool() const;
	
	inline bool operator < ( const SmartPtr & other ) const;
	inline bool operator > ( const SmartPtr & other ) const;
	inline bool operator <= ( const SmartPtr & other ) const;
	inline bool operator >= ( const SmartPtr & other ) const;
	inline bool operator == ( const SmartPtr & other ) const;
	inline bool operator != ( const SmartPtr & other ) const;
	
	inline SmartPtr<T>& operator = ( T & value );
	inline SmartPtr<T>& operator = ( const T & value );
	inline SmartPtr<T>& operator = ( T * ptr );		// argument to these operator shouldn't be stored after calling this function
	inline SmartPtr<T>& operator = ( const SmartPtr<T> & other );
	inline SmartPtr<T>& operator = ( SmartPtr<T> & other );
	
	inline T* operator->();
	
	inline void Separate();
	
	void Delete();
	
	
	SmartPtr( const SmartPtr<T> & other );
	explicit SmartPtr( SmartPtr<T> & other );
	explicit SmartPtr();
	~SmartPtr();
};

//#include <SmartPtr.tcc>

#endif

