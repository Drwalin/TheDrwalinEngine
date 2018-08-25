
#ifndef SMART_PTR_H
#define SMART_PTR_H

#include <new>
#include <cstdio>

template < class T >
class SmartPtr;

template < class T >
class SmartPtrData final
{
private:
	
	T * data;
	SmartPtr<T> * oneOfThem;
	
	friend SmartPtr<T>;
	
	void PureRemoveMember( SmartPtr<T>* member )
	{
		SmartPtr<T>* prev(member->prev);
		SmartPtr<T>* next(member->next);
		
		member->next = member;
		member->prev = member;
		member->data = nullptr;
		
		if( prev == member )
		{
			DestroyGroup();
			return;
		}
		else
		{
			prev->next = next;
			next->prev = prev;
		}
	}
	
	static void PureDestroyGroup( SmartPtrData<T> * ptr )
	{
		SmartPtr<T> * current( ptr->oneOfThem );
		SmartPtr<T> * next;
		
		if( ptr->data )
			delete ptr->data;
		
		while( true )
		{
			next = current->next;
			
			current->next = current;
			current->prev = current;
			current->data = nullptr;
			
			if( next == ptr->oneOfThem )
				break;
			
			current = next;
			next = current->next;
		}
		
		ptr->oneOfThem = nullptr;
	}
	
public:
	
	void AddMember( SmartPtr<T>* member )
	{
		member->prev = oneOfThem;
		member->next = oneOfThem->next;
		
		oneOfThem->next->prev = member;
		oneOfThem->next = member;
		
		member->data = this;
	}
	
	void RemoveMember( SmartPtr<T>* member )
	{
		if( oneOfThem == member )
		{
			if( member->next == member )
			{
				DestroyGroup();
				return;
			}
			else
			{
				oneOfThem = member->next;
			}
		}
		
		PureRemoveMember( member );
	}
	
	void DestroyGroup()
	{
		delete this;
	}
	
	SmartPtrData( SmartPtr<T>* member ) :
		data(nullptr), oneOfThem(member)
	{
		member->next = member;
		member->prev = member;
	}
	
	~SmartPtrData()
	{
		PureDestroyGroup( this );
		printf( "\n ~SmartPtrData() " );
	}
};



template < class T >
class SmartPtr final
{
private:
	
	SmartPtrData<T> * data;
	SmartPtr<T> * next;
	SmartPtr<T> * prev;
	
	friend SmartPtrData<T>;
	
	inline void InitData()
	{
		if( data == nullptr )
			data = new SmartPtrData<T>( this );
	}
	
public:
	
	inline const T* GetPtr() const
	{
		if( data )
			return data->data;
		return nullptr;
	}
	
	inline operator bool()
	{
		InitData();
		return bool( data->data != nullptr );
	}
	
	
	
	inline SmartPtr<T>& operator = ( T & value )
	{
		InitData();
		if( data->data )
			*(data->data) = value;
		else
			data->data = new T( value );
		return *this;
	}
	
	inline SmartPtr<T>& operator = ( const T & value )
	{
		InitData();
		if( data->data == nullptr )
			data->data = new T( value );
		else
			*(data->data) = value;
		return *this;
	}
	
	inline SmartPtr<T>& operator = ( T * ptr )		// argument to these operator shouldn'y be stored after calling this function
	{
		InitData();
		if( data->data )
			delete data->data;
		data->data = ptr;
		return *this;
	}
	
	inline SmartPtr<T>& operator = ( SmartPtr<T> * allocator )
	{
		InitData();
		if( data->data )
			delete data->data;
		data->data = nullptr;
		
		if( allocator )
		{
			if( allocator->data )
			{
				data->data = allocator->data->data;
				allocator->data->data = nullptr;
			}
			delete allocator;
		}
		
		return *this;
	}
	
	inline SmartPtr<T>& operator = ( SmartPtr<T> & other )
	{
		if( data )
			data->RemoveMember( this );
		other.InitData();
		other.data->AddMember( this );
	}
	
	inline T* operator->()
	{
		InitData();
		if( data->data == nullptr )
			New();
		return data->data;
	}
	
	inline void Separate()
	{
		if( data )
			data->RemoveMember( this );
	}
	
	template < typename... Args >
	inline void New( Args... args )
	{
		InitData();
		/*
		if( data->data )
			delete data->data;
		data->data = new T( args... );
		*/
		if( data->data )
		{
			data->data->~T();
			new (data->data) T( args... );
		}
		else
			data->data = new T( args... );
	}
	
	inline void Delete()
	{
		if( data )
		{
			if( data->data )
			{
				delete data->data;
				data->data = nullptr;
			}
		}
	}
	
	template < typename... Args >
	explicit SmartPtr( Args... args ) :
		data( new SmartPtrData<T>( this ) )
	{
		data->data = new T( args... );
	}
	
	explicit SmartPtr( SmartPtr<T> & other )
	{
		other.InitData();
		other.data->AddMember( this );
	}
	
	explicit SmartPtr() :
		data( new SmartPtrData<T>( this ) )
	{
	}
	
	~SmartPtr()
	{
		data->RemoveMember( this );
		printf( "\n ~SmartPtr() " );
	}
};


#endif

