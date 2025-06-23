#include <cstdint>
#include <string>
#include <fstream>
#include <iterator>
#include <vector>
#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <string.h>
#include <cstddef>
#include <filesystem>
#include <iostream>

extern "C" {
#include "idx_error_c.h"
#include "idx_memory_c.h"
#include "idx_result_c.h"
#include "idx_type_data_c.h"
}

#include "idx_error_cpp.h"
#include "idx.hpp"

Idx::Idx(const Idx& ctor) :
	type(ctor.type),
	number_of_elements(ctor.number_of_elements),
	element_size(ctor.element_size),
	type_size(ctor.type_size)
{

	payload = std::make_unique<std::uint8_t[]>(ctor.number_of_elements * ctor.element_size);
	memcpy(payload.get(), ctor.payload.get(), ctor.number_of_elements * ctor.element_size);
	dimension = ctor.dimension;
}

Idx::Idx(std::string& filename) : Idx{filename.c_str()} {}

Idx::~Idx() {} // Created as tests

Idx::Idx(Idx* parent, size_t begin, size_t end) {

	if(begin > end || begin > parent->number_of_elements) { throw std::runtime_error("Invalid index"); }

	type = parent->type;
	dimension = parent->dimension;
	number_of_elements = end - begin;
	element_size = parent->element_size;
	type_size = parent->type_size;

	payload = std::make_unique<std::uint8_t[]>(number_of_elements * element_size);

	memcpy(payload.get(), parent->payload.get() + begin * element_size, element_size * number_of_elements);
}

Idx::Idx(const char* filename) {

	try{
		// check if file exists
		if(!std::filesystem::exists(filename)){
			throw std::runtime_error(std::string("Cannot Open [")+filename+"], file not found\n");
		}
	
		struct idx_result memory = idx_memory_from_filename(filename);
		if(memory.error) {
			throw std::runtime_error(idx::idx_get_error_as_string(memory.error));
		}
	
		struct idx_memory* mem = memory.memory;
	
		// Pass from C to C++
		element_size = mem->element_size;
		payload = std::make_unique<std::uint8_t[]>(mem->number_of_elements * mem->element_size);
		memcpy(payload.get(), mem->element, mem->number_of_elements * mem->element_size);
	
		number_of_elements = mem->number_of_elements;
		type = mem->type;
		type_size = idx_type_data_size(type);
		dimension.reserve(mem->number_of_dimensions);
		for(size_t i = 0; i < mem->number_of_dimensions; i++) {
			dimension.push_back(mem->dimension[i]);
		}
	
		idx_result_free(memory);

	}catch(std::runtime_error& error){

		std::cerr << "Error : " << error.what() << '\n';
		exit(EXIT_FAILURE);

	}
}

Idx::operator uint8_t*() {
	try{
		if(type != UNSIGNED_8_INT) {
			throw std::runtime_error("Cannot cast to uint8_t* because, the type is not UNSIGNED_8_INT\n");
		}
		return payload.get();
	}catch(std::runtime_error& error){
		std::cerr << "Error : " << error.what() << '\n';
		exit(EXIT_FAILURE);
	}
}

Idx Idx::operator[](size_t position) {
	return slice(position, position + 1);
}

Idx Idx::slice(size_t begin, size_t end) {
	try{
		return Idx(this, begin, end);
	}catch(std::runtime_error& error){
		std::cerr << "Error : " << error.what() << '\n';
		exit(EXIT_FAILURE);
	}
}
