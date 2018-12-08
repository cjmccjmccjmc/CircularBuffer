/*
 CircularBuffer.h - Circular buffer library for Arduino.
 Copyright (c) 2017 Roberto Lo Giacco.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as 
 published by the Free Software Foundation, either version 3 of the 
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_
#include <stdint.h>
#include <stddef.h>

#ifdef CIRCULAR_BUFFER_DEBUG
#include <Print.h>
#endif

namespace Helper {
	template<bool XS> struct Index {
		using Type = uint16_t;
	};

	template<> struct Index<true> {
		using Type = uint8_t;
	};
}

template<typename T, size_t S, typename IT = typename Helper::Index<(S <= UINT8_MAX)>::Type> class CircularBuffer {
public:
	static constexpr IT capacity = static_cast<IT>(S);

	constexpr CircularBuffer();

	/**
	 * Adds an element to the beginning of buffer: the operation returns `false` if the addition caused overwriting an existing element.
	 */
	bool unshift(T value);

	/**
	 * Adds an element to the end of buffer: the operation returns `false` if the addition caused overwriting an existing element.
	 */
	bool push(T value);

	/**
	 * Removes an element from the beginning of the buffer.
	 */
	T shift();

	/**
	 * Removes an element from the end of the buffer.
	 */
	T pop();

	/**
	 * Returns the element at the beginning of the buffer.
	 */
	T inline first();

	/**
	 * Returns the element at the end of the buffer.
	 */
	T inline last();

	/**
	 * Array-like access to buffer
	 */
	T operator [] (IT index);

	/**
	 * Returns how many elements are actually stored in the buffer.
	 */
	IT inline size();

	/**
	 * Returns how many elements can be safely pushed into the buffer.
	 */
	IT inline available();

	/**
	 * Returns `true` if no elements can be removed from the buffer.
	 */
	bool inline isEmpty();

	/**
	 * Returns `true` if no elements can be added to the buffer without overwriting existing elements.
	 */
	bool inline isFull();

	/**
	 * Resets the buffer to a clean status, making all buffer positions available.
	 */
	void inline clear();

	#ifdef CIRCULAR_BUFFER_DEBUG
	void inline debug(Print* out);
	void inline debugFn(Print* out, void (*printFunction)(Print*, T));
	#endif

private:
	T buffer[S];
	T *head;
	T *tail;
#ifndef CIRCULAR_BUFFER_INT_SAFE
	IT count;
#else
	volatile IT count;
#endif
};

#include <CircularBuffer.tpp>
#endif
