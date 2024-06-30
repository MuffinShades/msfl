#include "pch.h"
#include "RawBytes.hpp"


void msfl::RawBytes::CopyToBuffer(msfl::RawBytes* buffer) {
	if (buffer != nullptr && this->len > 0 && this->bytes != nullptr) {
		buffer->bytes = new unsigned char[this->len];
		buffer->len = this->len;

		memcpy(buffer->bytes, this->bytes, this->len * sizeof(unsigned char));
	}
}

//function to clear a raw bytes structure
void msfl::RawBytes::deleteMemory() {
	if (this->bytes != nullptr && this->len > 0) {
		delete[] this->bytes;
		this->len = 0;
	}
}