#pragma once

#include <stdint.h>
#include <stddef.h>

class BaseNetworkValue
{
public:
	virtual const uint8_t* to_char_array() const = 0;
	virtual size_t size() const = 0;
private:
};

template <typename T>
class NetworkValue : public BaseNetworkValue
{
public:
	NetworkValue(T value)
	{
		_data.object = value;
	}
    
	const T& value() const { return _data.object; }
    
	virtual const uint8_t* to_char_array() const
	{
		return _data.char_array;
	}
    
	virtual size_t size() const
	{
		return sizeof(T);
	}
private:
	union
	{
		T object;
		uint8_t char_array[sizeof(T)];
	} _data;
};

template <typename T>
class NetworkArray : public BaseNetworkValue
{
public:
	NetworkArray(const T* value, const size_t N) : _data(new T[N]), _count(N)
	{
		memcpy(_data, value, size());
	}
    
	~NetworkArray()
	{
		delete _data;
	}
    
	const T* value() const
	{
		return _data;
	}
    
	virtual const uint8_t* to_char_array() const
	{
		return reinterpret_cast<const uint8_t*>(_data);
	}
    
	virtual size_t size() const
	{
		return _count * sizeof(T);
	}
private:
	T* _data;
	const size_t _count;
};