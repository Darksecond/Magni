#pragma once

#include "Networkvalue.h"

#include <vector>
#include <memory>
#include <stdexcept>
#include <cstring>

class NetworkPacket
{
public:
	NetworkPacket() : _size(0), _count(0), _array(nullptr), _values(), _dirty(true), _readonly(false)
	{
	}

	explicit NetworkPacket(const uint8_t* array, size_t size) : _size(size), _count(0), _array(new uint8_t[size]), _values(), _dirty(false), _readonly(true)
	{
		memcpy(_array, array, size);

		build_count_from_array();
	}

	~NetworkPacket()
	{
		delete _array;
	}

	template <typename T>
	void set(int index, T value)
	{
		if(_readonly) throw std::runtime_error("this object is in read-only mode");
		if(index >= _values.size())
		{
			_values.resize(index+1);
			_count = index + 1;
		}

		_values.at(index) = std::move(std::unique_ptr<NetworkValue<T>>(new NetworkValue<T>(value)));
		_dirty = true;
	}

	template <typename T>
	void build_value_from_array(size_t index)
	{
		int j = 0;
		for(uint8_t* i = _array + sizeof(size_t); i < _array + _size;)
		{
			size_t value_size = *reinterpret_cast<size_t*>(i);
			i += sizeof(size_t);

			if(j == index)
			{
				if(index >= _values.size())
				{
					_values.resize(index+1);
				}

				T value = *reinterpret_cast<T*>(i);
				_values[index] = std::move(std::unique_ptr<NetworkValue<T>>(new NetworkValue<T>(value)));
			}
			i += value_size;
			++j;
		}
	}

	template <typename T>
	const T& get(int index)
	{
		if(index >= _count)
			throw std::runtime_error("you are trying to access an invalid index");

		if(index >= _values.size() || _values[index] == nullptr)
		{
			build_value_from_array<T>(index);
		}

		NetworkValue<T>* value = static_cast<NetworkValue<T>*>(_values[index].get());
		return value->value();
	}

	void build_array()
	{
		_size = sizeof(size_t); //header
		for(auto& ptr : _values)
		{
			if(ptr)
				_size += ptr->size();
		}
		_size += _count * sizeof(size_t);

		delete _array;
		_array = new uint8_t[_size]; 

		//build up _array
		int i = 0;

		//header
		
		//number of values
		union
		{
			size_t as_size_t;
			char as_char[sizeof(size_t)];
		} count;
		count.as_size_t = _count;
		memcpy(_array+i, count.as_char, sizeof(count));
		i += sizeof(count);
		
		for(auto& ptr : _values)
		{
			if(ptr)
			{
				//header for value
				union
				{
					size_t as_size_t;
					char as_char[sizeof(size_t)];
				} length;
				length.as_size_t = ptr->size();
				memcpy(_array+i, length.as_char, sizeof(length));
				i += sizeof(length);

				//body of value
				memcpy(_array+i, ptr->to_char_array(), ptr->size());
				i += ptr->size();
			}
			else
			{
				//header for null value (size 0)
				union
				{
					size_t as_size_t;
					char as_char[sizeof(size_t)];
				} length;
				length.as_size_t = 0;
				memcpy(_array+i, length.as_char, sizeof(length));
				i += sizeof(length);
			}
		}

		_dirty = false;
	}

	const uint8_t* char_array()
	{
		if(_dirty) build_array();
		return _array;
	}

	size_t size()
	{
		if(_dirty) build_array();
		return _size;
	}

	size_t count()
	{
		return _count;
	}
private:
	size_t _size;
	size_t _count;
	uint8_t* _array;
	std::vector<std::unique_ptr<BaseNetworkValue>> _values;
	bool _dirty;
	bool _readonly;

	void build_count_from_array()
	{
		_count = *reinterpret_cast<size_t*>(_array);
	}
};
