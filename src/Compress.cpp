#include "pch.h"
#include "Compress.h"
#include <algorithm>

template<class _T> void _SortVector(std::vector<_T>& _vec) {
	std::sort(_vec.begin(), _vec.end(), [](_T& a, _T& b) {

		if (a.count == b.count) {
			if (a.branch) return false;

			return true;
		}

		return a.count < b.count;
	});
}

template<typename _T> void _SortVector2(std::vector<_T>& _vec) {
	int swaps = 1;

	while (swaps > 0) {
		swaps = 0;

		for (int i = 0; i < _vec.size(); i++) {

		}
	}
}

//generate a tree
HuffmanTreeNode* GenerateTree(std::vector<CharCount> _charCount) {
	std::vector<HuffmanTreeNode> treeTemplate;

	std::copy(_charCount.begin(), _charCount.end(), std::back_inserter(treeTemplate));

	//create the tree
	while (treeTemplate.size() > 1) {
		//sort tree vector
		_SortVector(treeTemplate);

		//merge
		HuffmanTreeNode n1 = treeTemplate[0];
		HuffmanTreeNode n2 = treeTemplate[1];

		HuffmanTreeNode newNode;

		newNode.left = new HuffmanTreeNode();
		newNode.left->branch = n1.branch;
		newNode.left->count = n1.count;
		newNode.left->left = n1.left;
		newNode.left->right = n1.right;
		newNode.left->c = n1.c;

		newNode.right = new HuffmanTreeNode();
		newNode.right->branch = n2.branch;
		newNode.right->count = n2.count;
		newNode.right->left = n2.left;
		newNode.right->right = n2.right;
		newNode.right->c = n2.c;

		newNode.branch = true;
		newNode.count = n1.count + n2.count;
		
		treeTemplate.erase(treeTemplate.begin(), treeTemplate.begin() + 2);

		treeTemplate.push_back(newNode);
	}

	HuffmanTreeNode* res = new HuffmanTreeNode();

	HuffmanTreeNode temp = treeTemplate[0];

	res->branch = temp.branch;
	res->left = temp.left;
	res->right = temp.right;
	res->c = temp.c;
	res->count = temp.count;

	return res;
}

struct HuffmanPath {
	char c = NULL;
	std::vector<int> path;
	int pathLen = 0;
};

void GetCharPath(HuffmanTreeNode* targetNode, HuffmanPath currentPath, std::vector<HuffmanPath>& paths) {
	if (targetNode != nullptr) {
		if (targetNode->branch) {
			//create path to the left
			HuffmanPath lPath = currentPath;
			lPath.path.push_back(0x00);
			lPath.pathLen++;

			//create path to the right
			HuffmanPath rPath = currentPath;
			rPath.path.push_back(0x01);
			rPath.pathLen++;

			//traverse left and right paths
			GetCharPath(targetNode->left, lPath, paths);
			GetCharPath(targetNode->right, rPath, paths);
		}
		else {
			currentPath.c = targetNode->c;
			paths.push_back(currentPath);
		}
	}
}

void PrintTree(HuffmanTreeNode* tree,std::string delim = "") {
	if (tree->branch) {
		PrintTree(tree->left, delim + "-");
		PrintTree(tree->right, delim + "-");
	}
}

HuffmanResult Huffman::Encode(unsigned char* bytes, int len) {
	if (len <= 0 || bytes == nullptr) {
		HuffmanResult errRes;

		errRes.resultType == HUFFMAN_ERROR;

		return errRes;
	}

	std::vector<CharCount> _charCount;

	int nUniqueChars = 0;

	//get character counts
	for (int i = 0; i < len; i++) {
		bool f = false;
		for (int j = 0; j < _charCount.size(); j++) {
			if (_charCount[j].c == (char) * (bytes + i)) {
				_charCount[j].count++;
				f = true;
			}
		}

		if (!f) {
			CharCount newCount;

			newCount.c = (char) * (bytes + i);
			newCount.count = 1;
			nUniqueChars++;
			_charCount.push_back(newCount);
		}
	}

	//generate the tree
	HuffmanTreeNode* treeHead = GenerateTree(_charCount);

	PrintTree(treeHead);

	//generate link table
	std::vector<HuffmanPath> charPaths;
	HuffmanPath iPath;
	GetCharPath(treeHead, iPath, charPaths);

	//generate output
	HuffmanResult res;

	int currentByte = 0x00;

	int nBits = 0;

	int bitCount = 0;

	std::vector<unsigned char> _bytes;

	//get resulting bytes
	//std::cout << "NEW BIT: ";
	for (int i = 0; i < len; i++) {
		char b = *(bytes+i);

		for (int j = 0; j < charPaths.size(); j++) {
			if ((char) charPaths[j].c == b) {
				nBits += charPaths[j].path.size();

				for (int k = 0; k < charPaths[j].path.size(); k++) {
					//std::cout << charPaths[j].path[k];
					currentByte |= (charPaths[j].path[k] << bitCount);

					bitCount++;

					if (bitCount >= 8) {
						bitCount = 0;
						//std::cout << "                     OUT BYTE: " << (int) currentByte << std::endl;
						_bytes.push_back(currentByte);
						currentByte = 0x00;
					}
				}

				break;
			}
		}
	}

	_bytes.push_back(currentByte);

	
	//counstruct result

	res.len = _bytes.size();

	res.bytes = new unsigned char[_bytes.size()];
	memcpy(res.bytes, _bytes.data(), _bytes.size() * sizeof(unsigned char));
	_bytes.clear();
	
	res.bitLen = nBits;
	
	res.charCount = _charCount;
	res.bitOverflow = bitCount;
	res.compressionRatio = (float)nBits / (float)(len * 8);
	res.resultType = HUFFMAN_RESULT_DECODE;

	return res;
}


//extract a specific bit from a bunch of bytes
uint32_t ExtractBitByte(unsigned char* _bytes, long bit) {
	int cByte = floor(bit / 8),
		byte = (uint32_t)*(_bytes + cByte),
		bbit = bit - (cByte * 8);
	return (uint32_t)((byte >> bbit) & 0x00000001);
}

//fnuction to extract a character from a tree
char _ExtractCharFromTree(HuffmanTreeNode* _tree, unsigned char* _bytes, long& _nBit) {
	uint32_t _bit = ExtractBitByte(_bytes, _nBit);
	
	//traverse tree
	if (_tree != nullptr) {
		if (_tree->branch) {
			switch (_bit) {
				case 0: {
					_nBit++;
					return _ExtractCharFromTree(_tree->left, _bytes, _nBit);
				}
				case 1: {
					_nBit++;
					return _ExtractCharFromTree(_tree->right, _bytes, _nBit);
				}
				default: {
					return NULL;
				}
			}
		}
		else {
			return _tree->c;
		}
	}
	
	return NULL;
}

//decode function
HuffmanResult Huffman::Decode(HuffmanResult res) {
	//first construct the tree from the result

	std::vector<CharCount> _vCharCount = res.charCount;

	HuffmanTreeNode* treeHead = GenerateTree(_vCharCount);

	//std::cout << "----Tree----" << std::endl;

	//PrintTree(treeHead);

	std::vector<unsigned char> res_byte_vec;
	//std::cout << "Got Bit " << std::hex << (int)res.bytes << "   " << std::endl;
	/*for (int i = 0; i < res.bitLen; i++) {
		//std::cout << ExtractBitByte(&res.bytes, i);
	}*/
	//std::cout << std::endl;

	//then decode
	long _cBit = 0;
	int nChars = 0;

	while (_cBit < res.bitLen) {
		//extract the next character in the data
		char _extractedChar = _ExtractCharFromTree(treeHead, res.bytes, _cBit);
		//std::cout << "GOT CHAR " << _extractedChar << std::endl;
		res_byte_vec.push_back((unsigned char)_extractedChar);
		nChars++;
	}

	//delete[] res.bytes;

	//construct huffman result
	HuffmanResult _res;

	_res.len = nChars;

	_res.bytes = new unsigned char[nChars];
	memcpy(_res.bytes, res_byte_vec.data(), nChars * sizeof(unsigned char));
	res_byte_vec.clear();

	_res.bitOverflow = NULL;
	_res.resultType = HUFFMAN_RESULT_DECODE;
	_res.nChars = NULL;
	_res.bitLen = _res.len * 8;
	_res.compressionRatio = 1.0f;

	return _res;
}

//decode bytes
HuffmanResult Huffman::Decode(unsigned char* bytes, int len) {

	//length check error
	if (len <= 0 || bytes == nullptr) {
		HuffmanResult errRes;

		errRes.resultType == HUFFMAN_ERROR;

		return errRes;
	}

	//construct a huffman result from bytes
	int bitOverflow = 0;
	int nChars = 0;
	int dataStart = 0x00;

	//std::cout << "EXTRACT LEN: " << len << "   " << (uintptr_t*)bytes << std::endl;

	//extract header info
	if (len > 2 && (uintptr_t*) bytes != nullptr) {
		try {

			//fetch info
			bitOverflow = (((unsigned char)*(bytes)) & 0xf0) >> 4; //4 bits for bit overflow
			nChars = (((unsigned char) * (bytes) & 0x0f) << 8) | (unsigned char)*(bytes + 1);

			//NOTE there are 5 bytes per character in the table
			dataStart = 2 + nChars * 5; //so data starts at 2 + nChars * 5;
		}
		catch (std::exception e) {
			HuffmanResult _nullRes;
			_nullRes.resultType = HUFFMAN_ERROR;

			return _nullRes;
		}
	}
	else {
		HuffmanResult _nullRes;
		_nullRes.resultType = HUFFMAN_ERROR;

		return _nullRes;
	}

	//now extract raw data
	int rawDatLen = len - dataStart; 

	//std::cout << "RAW DAT LEN: " << rawDatLen << std::endl;
	HuffmanResult res;

	//allocate result memory
	res.bytes = new unsigned char[rawDatLen];

	//just turned for loop into a memcpy
	memcpy(res.bytes, bytes + dataStart, rawDatLen * sizeof(unsigned char));

	//now extract table
	int tableExtractionStart = 2;
	int tableExtractionEnd = dataStart;

	//std::cout << "TABLE EXTRACT: " << tableExtractionStart << "-" << tableExtractionEnd << std::endl;

	std::vector<CharCount> _charTable;

	for (int i = tableExtractionStart; i < tableExtractionEnd; i += 5) {
		CharCount currentCount;

		currentCount.c = (char)*(bytes + i);
		
		int _c = 0;

		//extract count int from the headers
		for (int j = 4; j >= 1; j--) {
			_c |= (*(bytes + i + j) << ((j-1)*8));
		}

		currentCount.count = _c;

		//std::cout << "Char Node: " << currentCount.c << "  " << currentCount.count << std::endl;

		_charTable.push_back(currentCount);
	}

	//default return delete when finishing function

	//bytes


	res.len = rawDatLen - 1;

	res.charCount = _charTable;
	res.bitLen = (res.len * 8) + bitOverflow;
	res.bitOverflow = bitOverflow;
	res.compressionRatio = 1.0f;
	res.nChars = HUFFMAN_RESULT_ENCODE;

	//std::cout << "DBITLEN: " << res.bitLen << "   " << rawDatLen * 8 << "   " << bitOverflow << std::endl;

	return Huffman::Decode(res);
}

//
RawBytes HuffmanResult::GetFullByteResult() {
	//get char count table vector
	std::vector<unsigned char> _charCount;

	//append bit overflow
	unsigned char _bob = (unsigned char)this->bitOverflow;

	//first append char counut
	unsigned short _cCount = (unsigned short)this->charCount.size();

	unsigned char _cByte1 = (_bob << 4) | (unsigned char) ((_cCount & 0x100) >> 8);
	unsigned char _cByte2 = (unsigned char) (_cCount & 0xff);

	//std::cout << "BYTE 1: " << (unsigned int)_cByte1 << std::endl;
	//std::cout << "BYTE 2: " << (unsigned int)_cByte2 << std::endl;

	//std::cout << "OTHER INF: " << (unsigned int)_cCount << " " << (unsigned int)_bob << std::endl;

	_charCount.push_back(_cByte1);
	_charCount.push_back(_cByte2);
	

	//construct table
	for (int i = 0; i < this->charCount.size(); i++) {
		//std::cout << "Char: " << (int)(unsigned char)this->charCount[i].c << std::endl;
		_charCount.push_back((unsigned char) this->charCount[i].c); //add the count character

		//append the amount
		for (size_t j = 0; j < sizeof(int); j++) {
			_charCount.push_back((this->charCount[i].count >> (j * 8)) & 0xff);
		}
	}

	//allocate memory
	RawBytes res;

	res.bytes = new unsigned char[this->len + _charCount.size()];

	int cBit = 0;

	//first append count table data
	/*for (int i = 0; i < _charCount.size(); i++) {
		*(res.bytes + cBit) = _charCount[i];
		cBit++;
	}*/

	memcpy(res.bytes, _charCount.data(), _charCount.size());
	cBit += _charCount.size();

	//then append actual data
	/*for (int i = 0; i < this->len; i++) {
		*(res.bytes + cBit) = *(this->bytes+i);
		cBit++;
	}*/

	memcpy(res.bytes + cBit, this->bytes, this->len);
	cBit += this->len;
	
	res.len = this->len + _charCount.size();

	//finally return our blob of memory
	return res;
}

std::string BytePrinter::GetByteString(unsigned char* bytes) {
	return std::string((char*)bytes);
}

unsigned char* BytePrinter::GetBytesFromString(std::string text) {
	unsigned char* res = new unsigned char[text.length()];
	res = (unsigned char*) text.c_str();
	return res;
}

//lzss

template<typename _T> int SearchBufferChar(std::vector<_T> buffer, _T search) {
	for (int i = 0; i < buffer.size(); i++) {
		if (buffer[i] == search) return i;
	}

	return -1;
}

template<typename _T> int SearchBufferMChar(std::vector<_T> buffer, std::vector<_T> search) {
	if (search.size() > buffer.size()) return -1;

	for (int i = 0; i < buffer.size(); i++) {
		if (buffer[i] == search[0]) {
			bool s = true;

			for (int j = 1; j < search.size(); j++) {
				if (search[j] != buffer[i + j]) {
					s = false;
					break;
				}
			}

			if (s) {
				return i;
			}
		}
	}

	return -1;
}

//TODO add comments so you dont get a head ache, also drink some water
lzss_result lzss::compress(unsigned char* bytes, int len, lzss_settings s) {
	std::vector<int> out_bits;

	//get bits from a byte
	auto get_char_bits = [](char c) {
		int res[8] = {};
		for (int i = 0; i < 8; i++) {
			res[i] = ((1 << i) & c) >> i;
		}
		return res;
	};



	int s_len = len;
	int idx = 0;
	std::vector<unsigned char> search_buffer;
	//std::cout << "BUFFERS " << s.b_buffer << "  " << s.s_buffer << std::endl;
	while (idx < s_len) {

		unsigned char c = *(bytes+idx);

		int searchIdx = SearchBufferChar<unsigned char>(search_buffer, c);

		if (searchIdx >= 0) {
			int l = 0;
			int _idx, last_idx;
			last_idx = _idx = searchIdx;
			std::vector<unsigned char> Chars;

			Chars.push_back(c);

			while (_idx != std::string::npos && l < s.s_buffer) {
				if (idx + l >= len) break;
				l++;
				last_idx = _idx;
				Chars.push_back(bytes[idx + l]);
				_idx = SearchBufferMChar(search_buffer,Chars);
			}

			if (l * 8 > log2(s.b_buffer) + log2(s.s_buffer) + 1) {
				idx += l;
				if (l <= 1) l++;
				out_bits.push_back(1);
				for (int i = log2(s.b_buffer) - 1; i >= 0; i--) {
					//std::cout << "BIT asdf " << (((1 << i) & last_idx) >> i) << std::endl;
					out_bits.push_back(((1 << i) & last_idx) >> i);
				}
				for (int i = log2(s.s_buffer) - 1; i >= 0; i--) {
					//std::cout << "BIT asdf " << (((1 << i) & l) >> i) << std::endl;
					out_bits.push_back(((1 << i) & l) >> i);
				}

				//std::cout << "<" << last_idx << "," << l << ">" << std::endl;
			}
			else {
				int* bits = get_char_bits(c);
				search_buffer.push_back(c);
				if (search_buffer.size() >= s.b_buffer) {
					search_buffer.erase(search_buffer.begin());
				}
				out_bits.push_back(0);
				for (int i = 7; i >= 0; i--) {
					int __v = *(bits + i);
					if (__v != 0 && __v != 1) __v = 0;
					//std::cout << "BIT " << __v << std::endl;
					out_bits.push_back(__v);
				}
				idx++;
			}
		}
		else {
			int* bits = get_char_bits(c);
			search_buffer.push_back(c);
			if (search_buffer.size() >= s.b_buffer) {
				search_buffer.erase(search_buffer.begin());
			}
			out_bits.push_back(0);
			for (int i = 7; i >= 0; i--) {
				int __v = *(bits + i);
				if (__v != 0 && __v != 1) __v = 0;
				//std::cout << "BIT " << __v << std::endl;
				out_bits.push_back(__v);
			}
			idx++;
		}
	}
	auto push_to_vec_begin = [](int val, std::vector<int>& vec) {
		std::vector<int> r;
		r.push_back(val);
		for (int i = 0; i < vec.size(); i++) {
			r.push_back(vec[i]);
		}
		vec = r;
	};

	//std::cout << out_bits.size() << " Output Len | " << out_bits.size() << std::endl;

	s.BYTES_PER_BUFFER = 2;
	for (int i = 8 * s.BYTES_PER_BUFFER - 1; i >= 0; i--) {
		push_to_vec_begin((s.s_buffer >> i) & 0x00000001, out_bits);
	}
	for (int i = 8 * s.BYTES_PER_BUFFER - 1; i >= 0; i--) {
		push_to_vec_begin((s.b_buffer >> i) & 0x00000001, out_bits);
	}

	lzss_result res;

	std::vector<unsigned char> temp_bytes;

	int i = 0;
	//std::cout << out_bits.size() << " Output Len | " << out_bits.size() << std::endl;
	while (i < out_bits.size()) {
		unsigned char b = 0;
		for (int j = 0; j < 8; j++) {
			//std::cout << out_bits[i] << " O BIT " << i << std::endl;
			b = (b | (out_bits[i] << j));
			i++;
			if (i >= out_bits.size()) break;
		}
		temp_bytes.push_back(b);
	}

	//add length overflow
	temp_bytes.push_back((unsigned char)_MIN(temp_bytes.size() * 8 - out_bits.size(), 8));

	//convert temp bytes
	res.len = temp_bytes.size();

	res.bytes = new unsigned char[res.len]; //allocate memory
	memcpy(res.bytes, temp_bytes.data(), res.len); //copy over bytes

	temp_bytes.clear(); //clear temp vector

	res.comp_ratio = (((float)res.len) / ((float)len)) * 100.0f;
	return res;
}

lzss_result lzss::decompress(lzss_result dat, int BUFFER_BYTE_LEN) {
	int _bitOverflow = (int)dat.bytes[dat.len - 1]; //extract bit overflow

	int b_buf = 0, f_buf = 0; //buffers

	//search buffer
	std::vector<unsigned char> s_buf;

	//extract back buffer length
	for (int i = BUFFER_BYTE_LEN * 8; i >= 0; i--) {
		b_buf = ((b_buf << 0x00000001) | ExtractBitByte(dat.bytes, i));
	}

	//extract front buffer length
	for (int i = BUFFER_BYTE_LEN * 8; i >= 0; i--) {
		f_buf = ((f_buf << 0x00000001) | ExtractBitByte(dat.bytes, i + BUFFER_BYTE_LEN * 8));
	}

	//extract used bytes
	int startByte = BUFFER_BYTE_LEN * 2;
	int endByte = dat.len - 1;
	int nUseBytes = endByte - startByte;
	unsigned char* useBytes = new unsigned char[nUseBytes]; //bytes we use (allocate memory)
	int nUseBits = nUseBytes * 8 - _bitOverflow;

	//copy over bytes
	int byteCount = 0;
	for (int i = startByte; i < endByte; i++) {
		useBytes[byteCount] = *(dat.bytes + i);
		byteCount++;
	}

	//BUFFER_BYTE_LEN * 2 * 8 -> start bit
	//BUFFER_BYTE_LEN * 2 -> start bytes
	//dat.len - 1 -> end byte
	//_bits.size() - _bitOverflow -> end bit 

	lzss_result _r; //result
	std::vector<unsigned char> temp_bytes; //bytes

	//extract info
	int i = 0;
	while (i < nUseBits) {
		//determine what the first bit is
		int cBit = ExtractBitByte(useBytes, i);

		if (cBit == 0) {
			i++;

			//extract the byte from the bits
			unsigned char b = 0;
			for (int j = 0; j < 8; j++) {
				b = (b << 1) | ExtractBitByte(useBytes, i);
				i++; //increase bit
			}

			//add current character to search buffer
			unsigned char c = b;
			s_buf.push_back(c);

			//remove old characters
			if (s_buf.size() >= b_buf) {
				s_buf.erase(s_buf.begin()); //remove first element if we overflow the buffer
			}

			//add character to our bytes
			temp_bytes.push_back((unsigned char)c);
		}
		else if (cBit == 1) {

			//offset
			int iOff = i;
			i++;

			//find how many bits are needed for the max backrefernce length
			int l_b = log2(b_buf), l_f = log2(f_buf);
			
			
			int back_ref = 0; //back ref len

			//extract how long the backreference is
			for (int j = 0; j < l_b; j++) {
				back_ref = (back_ref << 0x00000001) | ExtractBitByte(useBytes, i);
				i++;
			}

			int len = 0; //how many bytes were copying

			//extract how many characters we are gonna print
			for (int j = 0; j < l_f; j++) {
				len = (len << 0x00000001) | ExtractBitByte(useBytes, i);
				i++;
			}
			
			//append all the bytes we get
			for (int j = back_ref; j < back_ref + len; j++) {
				temp_bytes.push_back((unsigned char)s_buf[j]);
			}
		}
	};

	//copy over bytes and length
	_r.len = temp_bytes.size(); //get len
	_r.bytes = new unsigned char[_r.len]; //allocate mem
	memcpy(_r.bytes, temp_bytes.data(), _r.len); //copy over bytes
	temp_bytes.clear(); //clear memory

	return _r;
}

lzss_result lzss::decompress(unsigned char* bytes, int len, int BUFFER_BYTE_LEN) {
	lzss_result cRes;

	cRes.bytes = bytes;
	cRes.len = len;
	cRes.comp_ratio = 1.0f;

	return lzss::decompress(cRes, BUFFER_BYTE_LEN);
}


/*

To add better functionality when computing back reference append characters were reading then remove when done? - i forget lol

*/

//MZIP

RawBytes mzip::Encode(unsigned char* bytes, int len, MZIP_SETTINGS settings) {
	if (len <= 0 || bytes == nullptr) {
		RawBytes _nullRes;
		return _nullRes;
	}

	//lz77 first
	lzss_settings _s = lzss_settings();

	_s.b_buffer = settings.b_buffer;
	_s.s_buffer = settings.s_buffer;
	_s.BYTES_PER_BUFFER = settings.BYTES_PER_BUFFER;

	lzss_result _lzf = lzss::compress(bytes, len, _s);

	unsigned char* t_bytes = nullptr;
	int tLen = 0;

	bool _lzss = true;

	if (_lzf.comp_ratio > 1.00f) {
		t_bytes = new unsigned char[len];
		memcpy(t_bytes, bytes, len);
		tLen = len;
		_lzss = false;
	}
	else {
		t_bytes = new unsigned char[_lzf.len];
		memcpy(t_bytes, _lzf.bytes, _lzf.len);
		tLen = _lzf.comp_ratio;
	}

	//huffman encode
	HuffmanResult hResult = Huffman::Encode(t_bytes, tLen);
	RawBytes _fullHResult = hResult.GetFullByteResult();

	RawBytes res;

	//copy over bytes
	res.bytes = new unsigned char[_fullHResult.len + 1];
	memcpy(res.bytes+1, _fullHResult.bytes, _fullHResult.len);
	res.len = _fullHResult.len + 1;
	delete[] _fullHResult.bytes;
	_fullHResult.len = 0;

	*(res.bytes) = (unsigned char)_lzss;

	delete[] hResult.bytes;
	delete[] _lzf.bytes;

	return res;
}

RawBytes mzip::Decode(unsigned char* bytes, int len, MZIP_SETTINGS settings) {
	RawBytes res;

	if (bytes != nullptr && len > 0) {
		bool _lzss = (bool)*(bytes);

		//copy over working bytes
		unsigned char* _bytes = new unsigned char[len - 1];
		memcpy(_bytes, bytes+1, len - 1); //copy over working bytes

		//huffman decode first
		HuffmanResult _hRes = Huffman::Decode(_bytes, len - 1);

		//check for lz77 step
		if (_lzss) {
			lzss_result lres = lzss::decompress(_hRes.bytes, _hRes.len, settings.BYTES_PER_BUFFER);

			res.bytes = new unsigned char[lres.len];
			res.len = lres.len;
			memcpy(res.bytes, lres.bytes, lres.len);
		}
		else {
			res.bytes = new unsigned char[_hRes.len];
			res.len = _hRes.len;
			memcpy(res.bytes, _hRes.bytes, _hRes.len);
		}
	}

	return res;
}

void RawBytes::CopyToBuffer(RawBytes* buffer) {
	if (buffer != nullptr && this->len > 0 && this->bytes != nullptr) {
		buffer->bytes = new unsigned char[this->len];
		buffer->len = this->len;

		memcpy(buffer->bytes, this->bytes, this->len * sizeof(unsigned char));
	}
}

//function to clear a raw bytes structure
void RawBytes::deleteMemory() {
	if (this->bytes != nullptr && this->len > 0) {
		delete[] this->bytes;
		this->len = 0;
	}
}