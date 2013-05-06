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
