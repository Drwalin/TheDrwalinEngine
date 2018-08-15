
#ifndef AR_HPP
#define AR_HPP

template < class T, int N >
class AR
{
private:
	
	T data[N];
	
public:
	
	inline T&operator[](int id)
	{
		return data[id];
	}
	
	AR()
	{
		memset( data, 0, N*sizeof(T) );
	}
	
	AR( T defaultValue )
	{
		for( int i = 0; i < N; ++i )
			data[i] = defaultValue;
	}
};

#endif

