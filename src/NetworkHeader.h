#pragma once

#include <stdint.h>
#include <stddef.h>

class NetworkHeader
{
public:
	NetworkHeader(const size_t& size, const size_t& count) : _size(size), _count(count), _array(nullptr), _dirty(true)
	{
	}

	NetworkHeader(const uint8_t* array) : _size(0), _count(0), _array(nullptr), _dirty(true)
	{
		build_headers_from_array(array);
	}

	~NetworkHeader()
	{
		if(_array)
			delete _array;
	}

	inline const size_t& size() { return _size; }
	inline const size_t& count() { return _count; }

	static inline size_t header_length()
	{
		return sizeof(size_t) + sizeof(size_t);
	}

	inline const uint8_t* to_char_array()
	{
		if(_dirty)
			build_array();
		return _array;
	}
private:
	size_t _size;
	size_t _count;
	uint8_t* _array;
	bool _dirty;

	void build_headers_from_array(const uint8_t* array)
	{
		int i = 0;
		_size = *reinterpret_cast<const size_t*>(array+i);
		i += sizeof(size_t);

		_count = *reinterpret_cast<const size_t*>(array+i);
		i += sizeof(size_t);
	}

	void build_array()
	{
		delete _array;
		_array = new uint8_t[header_length()];

		int i = 0;

		//size
		union
		{
			size_t as_size_t;
			char as_char[sizeof(size_t)];
		} size;
		size.as_size_t = _size;
		memcpy(_array+i, size.as_char, sizeof(size));
		i += sizeof(size);
		
		//number of values (count)
		union
		{
			size_t as_size_t;
			char as_char[sizeof(size_t)];
		} count;
		count.as_size_t = _count;
		memcpy(_array+i, count.as_char, sizeof(count));
		i += sizeof(count);

		_dirty = false;
	}
};
