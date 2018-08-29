
#ifndef SMART_PTR_CPP
#define SMART_PTR_CPP

#include <SmartPtr.h>

#include <cassert>

template < class T >
inline void SmartPtrData<T>::PureRemoveMember( SmartPtr<T>* member )
{
	if( member )
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
			assert( prev );
			assert( next );
			prev->next = next;
			next->prev = prev;
		}
	}
}

template < class T >
inline void SmartPtrData<T>::PureDestroyGroup( SmartPtrData<T> * ptr )
{
	if( ptr )
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
}

template < class T >
inline void SmartPtrData<T>::AddMember( SmartPtr<T>* member )
{
	if( member )
	{
		member->prev = oneOfThem;
		member->next = oneOfThem->next;
		
		oneOfThem->next->prev = member;
		oneOfThem->next = member;
		
		member->data = this;
	}
}

template < class T >
inline void SmartPtrData<T>::RemoveMember( SmartPtr<T>* member )
{
	if( member )
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
}

template < class T >
inline void SmartPtrData<T>::DestroyGroup()
{
	delete this;
}

template < class T >
SmartPtrData<T>::SmartPtrData( SmartPtr<T>* member ) :
	data(nullptr), oneOfThem(member)
{
	if( member )
	{
		member->next = member;
		member->prev = member;
	}
}

template < class T >
SmartPtrData<T>::~SmartPtrData()
{
	PureDestroyGroup( this );
}





template < class T >
inline int SmartPtr<T>::CountReferences() const
{
	int i = 1;
	
	SmartPtr<T> * curr = (SmartPtr<T>*)this;
	SmartPtr<T> * next = curr->next;
	
	while( next != this )
	{
		++i;
		curr = next;
		next = curr->next;
	}
	
	return i;
}

template < class T >
inline void SmartPtr<T>::InitData()
{
	if( data == nullptr )
		data = new SmartPtrData<T>( this );
}

template < class T >
inline bool SmartPtr<T>::IsAlone() const
{
	return ( next == this && prev == this ) || ( next == nullptr && prev == nullptr );
}

template < class T >
inline void SmartPtr<T>::JoinOtherToGroup( SmartPtr & other )
{
	other = (*this);
}


template < class T >
inline SmartPtr<T>& SmartPtr<T>::operator = ( T & value )
{
	if( data )
		if( data->data )
			*(data->data) = value;
	return *this;
}

template < class T >
inline SmartPtr<T>& SmartPtr<T>::operator = ( const T & value )
{
	if( data )
		if( data->data )
			*(data->data) = value;
	return *this;
}

template < class T >
inline SmartPtr<T>& SmartPtr<T>::operator = ( T * ptr )
{
	InitData();
	if( data->data )
		delete data->data;
	data->data = ptr;
	return *this;
}

template < class T >
inline SmartPtr<T>& SmartPtr<T>::operator = ( const SmartPtr<T> & other )
{
	if( data )
		data->RemoveMember( this );
	
	if( other.data )
	{
		((SmartPtr<T>*)(&other))->data->AddMember( this );
	}
	
	return *this;
}

template < class T >
inline SmartPtr<T>& SmartPtr<T>::operator = ( SmartPtr<T> & other )
{
	if( data )
		data->RemoveMember( this );
	other.InitData();
	other.data->AddMember( this );
	return *this;
}

template < class T >
inline void SmartPtr<T>::Separate()
{
	if( data )
		data->RemoveMember( this );
}

template < class T >
void SmartPtr<T>::Delete()
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



template < class T >
SmartPtr<T>::SmartPtr( const SmartPtr<T> & other ) :
	data(nullptr), prev(this), next(this)
{
	*this = other;
}

template < class T >
SmartPtr<T>::SmartPtr( SmartPtr<T> & other ) :
	prev(this), next(this)
{
	other.InitData();
	other.data->AddMember( this );
}

template < class T >
SmartPtr<T>::SmartPtr() :
	data( new SmartPtrData<T>( this ) ), prev(this), next(this)
{
}

template < class T >
SmartPtr<T>::~SmartPtr()
{
	if( data )
		data->RemoveMember( this );
}

#include <Object.h>
#include <Model.h>
#include <Texture.h>
#include <Camera.h>

template class SmartPtr<btTriangleIndexVertexArray>;
template class SmartPtr<CustomCollisionShapeData>;
template class SmartPtr<btCollisionObject>;
template class SmartPtr<btCollisionShape>;
template class SmartPtr<btRigidBody>;
template class SmartPtr<Texture>;
template class SmartPtr<Object>;
template class SmartPtr<Camera>;
template class SmartPtr<Model>;

#endif

