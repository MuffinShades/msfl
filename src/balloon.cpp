/**
 *
 * BALLOON - C++ lightweight zlib implementation
 *
 * Program written by muffinshades
 *
 * Copyright (c) 2024 muffinshades
 *
 * You can do what ever you want with the software but you must
 * credit the author (muffinshades) with the original creation
 * of the software. Idk what else to put here lmao.
 *
 * Balloon Notes:
 *
 * This library is a implementation of the zlib or Inflate / Deflate
 * compression algorithm.
 *
 * Right now compression speeds are around 4-6mb/s and decompression
 * speeds are much faster. This isn't the worlds fastest implementation,
 * but its decently fast and lightweight. One day I will improve the LZ77
 * hash functions, but for now it's gonna stay at around 5mb/s.
 *
 * This program should be able to function with any other inflate / deflate
 * implementation apart from the whole compression level calculations being
 * different since I didn't entirley implement lazy and good matches into
 * the lz77 functions. I also didn't add a whole fast version for everything
 * since this is a relativley light weight library. One day I do plan on adding
 * these functions and making a even better implementation of zlib.
 *
 */

#include "balloon.hpp"
#include <algorithm>

 //MACROS
#define INRANGE(v,m,x) (v >= m && v < x)
#define MIN(a, b) (a <= b ? a : b)
#define WINDOW_BITS 15
#define MIN_MATCH 3
#define MEM_LEVEL 8


/**
 *
 * BitStream Functions
 *
 * Deflate code starts after this
 *
 */

BitStream::BitStream(u32* bytes, size_t len) {
	this->bytes = new u32[len];
	memcpy(this->bytes, bytes, len * sizeof(u32));
	this->bsz = len * 8;
	this->sz = len;
	this->asz = sz;
	this->rPos = this->sz;
	this->pos = 0;
}

BitStream::BitStream(size_t len) {
	assert(len > 0);
	this->bytes = new u32[len];
	memset(this->bytes, 0, len * sizeof(u32));
	this->bsz = len * 8;
	this->asz = len;
	this->rPos = 0;
	this->sz = 0;
}

i32 BitStream::readBit() {
	if (this->lBit <= 0) {
		assert(this->pos < this->bsz&& this->pos >= 0);

		//advance a byte
		this->cByte = this->bytes[this->pos];
		this->pos++;
		this->lBit = 8;
	}

	this->lBit--;

	//extract le bit
	i32 bit = this->cByte & 1; //extract first bit
	this->cByte >>= 1; //advance a bit
	return bit; //return extracted bit
}

i32 BitStream::readByte() {
	this->lBit = 0;
	return this->bytes[this->pos++];
}

i32 BitStream::readNBits(i32 nBits) {
	u32 r = 0, c = 0;

	while (c < nBits) {
		i32 bit = this->readBit() & 1;
		r |= (bit << (c++));
	}
	return r;
}

i32 BitStream::tell() {
	return this->pos;
}

i32 BitStream::tellw() {
	return this->rPos;
}

void BitStream::seekw(i32 wPos) {
	assert(wPos >= 0 && wPos < this->asz);
	this->rPos = wPos;
}

void BitStream::seek(i32 pos) {
	assert(pos >= 0 && pos < this->asz);
	this->pos = pos;
}

void BitStream::checkWritePosition() {
	if (this->rPos >= this->asz)
		this->allocNewChunk();
}

i32 BitStream::readValue(size_t size) {
	i32 r = 0;
	for (size_t i = 0; i < size; i++)
		r |= (this->readByte() << (i * 8));

	return r;
}

u32* BitStream::bitsToBytes(u32* bits, size_t len) {
	u32* res = new u32[(u32)ceil((float)len / 8.0f)];

	i32 bCollect = 0, iBit = 0, cByte = 0;
	for (i32 i = len - 1; i >= 0; i--) {
		bCollect |= (bits[i]) << iBit;
		iBit++;
		if (iBit >= 8) {
			res[cByte++] = bCollect;
			bCollect = 0;
			iBit = 0;
		}
	}

	if (iBit > 0) res[cByte] = bCollect;

	return res;
}

u32* BitStream::bitsToBytes(u32 bits, size_t len) {
	u32* res = new u32[(u32)ceil((float)len / 8.0f)];

	i32 bCollect = 0, iBit = 0, cByte = 0;
	for (i32 i = len - 1; i >= 0; i--) {
		bCollect |= ((bits >> i) & 1) << iBit;
		iBit++;
		if (iBit >= 8) {
			res[cByte++] = bCollect;
			bCollect = 0;
			iBit = 0;
		}
	}

	if (iBit > 0) res[cByte] = bCollect;

	return res;
}

void BitStream::writeBit(u32 bit) {
	//now write bit
	this->bytes[this->rPos] |= ((bit & 1) << (this->rBit++)); //& with 1 to just get first bit
	//advance a byte if were out of range
	if (this->rBit >= 8) {
		this->rBit = 0;
		this->rPos++;
		this->sz++;
		this->checkWritePosition();
	}
}

void BitStream::writeNBits(u32* bits, size_t len) {
	for (size_t i = 0; i < len; i++)
		this->writeBit(bits[i]);
}

template<typename _T> void t_writeValue(BitStream *s, _T val) {
	size_t vsz = sizeof(_T) * 8;

	for (i32 i = 0; i < vsz; i++)
		s->writeBit((val >> i) & 1);
}

//dll template fix
void BitStream::writeValue(byte val) {
	t_writeValue(this, val);
}

void BitStream::writeValue(short val) {
	t_writeValue(this, val);
}

void BitStream::writeValue(i32 val) {
	t_writeValue(this, val);
}

void BitStream::writeValue(u32 val) {
	t_writeValue(this, val);
}

void BitStream::writeValue(i64 val) {
	t_writeValue(this, val);
}

void BitStream::writeValue(u64 val) {
	t_writeValue(this, val);
}

//
void BitStream::allocNewChunk() {
	this->asz += 0xffff;
	u32* tBytes = new u32[this->asz];
	memset(tBytes, 0, sizeof(u32) * this->asz);
	memcpy(tBytes, this->bytes, sizeof(u32) * this->sz);
	delete[] this->bytes;
	//this->bytes = new u32[this->asz];
	//memset(this->bytes, 0, sizeof(u32) * this->asz);
	//memcpy(this->bytes, tBytes, sizeof(u32) * this->sz);
	//delete[] tBytes;
	this->bytes = tBytes;
}

void BitStream::clip() {
	//wtf???
	u32* tBytes = new u32[this->asz];
	u32 osz = this->asz;
	memcpy(tBytes, this->bytes, this->asz);
	this->asz = this->sz;
	delete[] this->bytes;
	this->bytes = new u32[this->asz];
	memset(this->bytes, 0, sizeof(u32) * this->asz);
	memcpy(this->bytes, tBytes, osz);
	delete[] tBytes;
}

void BitStream::calloc(size_t sz) {
	if (this->bytes)
		delete[] this->bytes;

	this->bytes = new u32[sz];

	this->asz = sz;
	this->bsz = sz * 8;
	this->pos = this->rPos = this->lBit = this->cByte = this->sz = 0;

	i32 pos = 0, rPos = 0;
	i32 lBit = 0;
	u32 cByte = 0;
	u32 rBit = 8;
}

//#define LZ77_TESTING

template<typename _Ty> void ZeroMem(_Ty* mem, size_t len) {
	memset(mem, 0, sizeof(_Ty) * len);
}

void FreeTree(HuffmanTreeNode* tree) {
	if (tree != nullptr) {
		try {
			if (tree->left != nullptr) FreeTree(tree->left);
			if (tree->right != nullptr) FreeTree(tree->right);

			delete tree;
		}
		catch (std::exception e) {
			std::cout << "free fail ;-;" << std::endl;
		}
	}
}

template<typename _Ty> size_t BufLen(_Ty* buf) {
	size_t sz = 0;
	do {} while (*buf[sz++] != 0);
	return sz / sizeof(_Ty);
}

void PrintTree(HuffmanTreeNode* tree, std::string tab = "", std::string code = "") {
	if (tree == nullptr) return;
	if (tree->right == nullptr && tree->left == nullptr) {
		std::cout << tab << "  - " << tree->count << " " << code << " " << "  " << tree->val << "   " << (char)tree->val << std::endl;
		return;
	}

	std::cout << tab << "|_ " << tree->count << std::endl;
	if (tree->right != nullptr)
		PrintTree(tree->right, tab + " ", code + "1");

	if (tree->left != nullptr)
		PrintTree(tree->left, tab + " ", code + "0");
}

u32 DecodeSymbol(BitStream& stream, HuffmanTreeNode* tree) {
	HuffmanTreeNode* n = tree;

	while (n->left != nullptr || n->right != nullptr) {
		i32 b = stream.readBit();

		if (!!b) {
			if (n->right == nullptr) break;

			n = n->right;
		}
		else {
			if (n->left == nullptr) break;

			n = n->left;
		}
	}

	return n->val;
}

void _GenSymCodes(HuffmanTreeNode* _t, HuffmanTreeNode* node, i32 cCode = 0x00) {
	if (node->left != nullptr || node->right != nullptr) {
		if (node->left != nullptr)  _GenSymCodes(_t, node->left, (cCode << 1));
		if (node->right != nullptr) _GenSymCodes(_t, node->right, (cCode << 1) | 0x1);
	}
	else if (node->val < _t->alphabetSz)
		_t->symCodes[node->val] = cCode;
}

/**
 *
 * Generate Code Table
 *
 * Function to generate all character codes based off a given tree.
 * This must be called on a tree before you call Encode Symbol because
 * all Encode Symbol does is reference this table.
 *
 * *Note if this is not
 * called then EncodeSymbol will call it for you.
 *
 * If there is no good alphabet size then the alphabet size will be set
 * to DEFAULT_ALPHABET_SIZE (defined below)
 *
 * [HuffmanTreeNode*] tree -> huffman tree to generate code table for
 * [size_t] aSize -> size of the tree's alphabet
 *
 */

#define DEFAULT_ALPHABET_SIZE 288

 //
void GenerateCodeTable(HuffmanTreeNode* tree, size_t aSize = 0) {
	if (aSize <= 0)
		aSize = DEFAULT_ALPHABET_SIZE;

	tree->alphabetSz = aSize;

	//allocate code table
	if (tree->symCodes != nullptr)
		delete[] tree->symCodes;

	tree->symCodes = new u32[aSize];
	ZeroMem(tree->symCodes, aSize);

	//generate codes
	_GenSymCodes(tree, tree);
}

/**
 *
 * EncodeSymbol
 *
 * Function to encode a symbol with a given huffman tree. This
 * is used by deflate for the huffman coding.
 *
 * [u32] sym -> symbol to be encoded
 * [HuffmanTreeNode*] tree -> huffman tree to encode the symbol
 * with
 *
 */

u32 EncodeSymbol(u32 sym, HuffmanTreeNode* tree) {
	assert(sym < tree->alphabetSz);

	if (tree->symCodes == nullptr)
		GenerateCodeTable(tree, tree->alphabetSz);

	return tree->symCodes[sym];
}

/**
 *
 * InsertNode
 *
 * Function used to insert values into a huffman tree during
 * the Inflate process. This is to construct the tree from a
 * list of codes generated by blListToTree
 *
 */

void InsertNode(u32 code, size_t codeLen, u32 val, HuffmanTreeNode* tree) {
	HuffmanTreeNode* current = tree;

	for (int i = codeLen - 1; i >= 0; i--) {
		int bit = (code >> i) & 1;

		if (!!bit) {
			if (current->right == nullptr)
				current->right = new HuffmanTreeNode();

			current = current->right;
		}
		else {
			if (current->left == nullptr)
				current->left = new HuffmanTreeNode();

			current = current->left;
		}
	}

	current->val = val;
}

/*

Python thing:

a[b:] creates an array with everything from a[b] - a[len-1]
a[:b] creates an array with everything from a[0] - a[b-1]


*/

i32 aMax(u32* a, size_t len) {
	i32 max = 0;

	for (size_t i = 0; i < len; i++) {
		if (a[i] > max) max = a[i];
	}

	return max;
}

/*

Function to get the number of occuraces and bit length appears

*/
i32* getBitLenCounts(u32* bitLens, size_t blLen, i32 MAX_BITS) {
	i32* res = new i32[MAX_BITS + 1];

	memset(res, 0, (MAX_BITS + 1) * sizeof(i32));
	for (int i = 0; i <= MAX_BITS; i++) {
		for (int j = 0; j < blLen; j++) {
			if (i == bitLens[j]) res[i] ++;
		}
	}

	return res;
}

HuffmanTreeNode* BitLengthsToHTree(u32* bitLens, size_t blLen, u32* alphabet, size_t aLen) {
	i32 MAX_BITS = aMax(bitLens, blLen);
	i32* blCount = getBitLenCounts(bitLens, blLen, MAX_BITS);

	std::vector<int> nextCode = { 0,0 };

	//get the next code for the tree items
	for (int codeIdx = 2; codeIdx <= MAX_BITS; codeIdx++) {
		nextCode.push_back((nextCode[codeIdx - 1] + blCount[codeIdx - 1]) << 1);
	}

	//construct tree
	int i = 0;
	HuffmanTreeNode* res = new HuffmanTreeNode();

	while (i < aLen && i < blLen) {
		if (bitLens[i] != 0) {
			InsertNode(nextCode[bitLens[i]], bitLens[i], alphabet[i], res);
			nextCode[bitLens[i]] ++;
		}
		i++;
	}

	//free and return
	delete[] blCount;

	return res;
}

const i32 CodeLengthCodesOrder[] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
const char CodeLengthAlphabet[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };

//to construct the codes for the huffman alphabets
unsigned int* CreateAlphabet(int len) {
	unsigned int* res = new unsigned int[len];

	for (int i = 0; i < len; i++)
		res[i] = i;

	return res;
}

std::vector<u32> _treeLens;

//
HuffmanTreeNode** decodeTrees(BitStream& stream) {
	i32 nTreeCodes = stream.readNBits(5) + 257; //main tree
	i32 nDistCodes = stream.readNBits(5) + 1; //distance tree
	i32 nClCodes = stream.readNBits(4) + 4; //tree tree

	//allocate and get get bit lens
	u32* bitLens = new u32[19];
	memset(bitLens, 0, sizeof(u32) * 19);

	//create bit lengths for the decode tree tree
	for (int i = 0; i < nClCodes; i++)
		bitLens[CodeLengthCodesOrder[i]] = stream.readNBits(3);

	//create the code length tree
	HuffmanTreeNode* codeLenTree = BitLengthsToHTree(bitLens, 19, (u32*)CreateAlphabet(19), 19);


#ifdef BALLOON_DEBUG
	std::cout << "CL TREE: " << std::endl;
	PrintTree(codeLenTree);
#endif

	//decode le trees
	std::vector<u32> combinedBitLens;

	while (combinedBitLens.size() < nTreeCodes + nDistCodes) {

		i32 sym = DecodeSymbol(stream, codeLenTree);

		//0-15 normal symbol
		if (sym <= 15) {
			combinedBitLens.push_back(sym);
		}
		//16 copy last 3-6 times
		else if (sym == 16) {
			i32 copyLen = stream.readNBits(2) + 3;
			i32 prev = combinedBitLens[combinedBitLens.size() - 1];

			for (int i = 0; i < copyLen; i++)
				combinedBitLens.push_back(prev);
		}
		//17 copy 0 3-10 times
		else if (sym == 17) {
			i32 copyLen = stream.readNBits(3) + 3;
			
			for (int i = 0; i < copyLen; i++)
				combinedBitLens.push_back(0);
		}
		//18 copy 0 11 - 138 times
		else if (sym == 18) {
			i32 copyLen = stream.readNBits(7) + 11;
			
			for (int i = 0; i < copyLen; i++)
				combinedBitLens.push_back(0);
		}
		else {
			std::cout << "Invalid Symbol Found! " << std::endl;
		}
	}
	std::cout << std::endl;

#ifdef BALLOON_DEBUG
	std::cout << "Combined Bits Lens: ";

	for (auto v : combinedBitLens) {
		std::cout << v;
	}

	std::cout << std::endl;
#endif

	//construct trees
	std::vector<u32> distLens, treeLens;

	for (int i = 0; i < combinedBitLens.size(); i++) {
		if (i < nTreeCodes) {
			treeLens.push_back(combinedBitLens[i]);
			_treeLens.push_back(combinedBitLens[i]);
		}
		else
			distLens.push_back(combinedBitLens[i]);
	}

	u32* da = (unsigned*)CreateAlphabet(30);
	u32* dm = (unsigned*)CreateAlphabet(286);

#ifdef BALLOON_DEBUG
	for (int i = 0; i < treeLens.size(); i++)
		if (treeLens[i] > 0) std::cout << "Bit Len: " << treeLens[i] << "  " << i << "   " << (char)i << std::endl;

	for (int i = 0; i < treeLens.size(); i++)
		if (treeLens[i] > 0) std::cout << treeLens[i];

	std::cout << std::endl;
#endif

	//create le trees
	HuffmanTreeNode* distTree = BitLengthsToHTree(distLens.data(), distLens.size(), da, 30);
	HuffmanTreeNode* mainTree = BitLengthsToHTree(treeLens.data(), treeLens.size(), dm, 286);

#ifdef BALLOON_DEBUG
	std::cout << "Trees: " << std::endl;
	PrintTree(distTree);
	std::cout << "-----------------------" << std::endl;
	PrintTree(mainTree);
#endif

	//free memory
	delete[] bitLens;
	FreeTree(codeLenTree);
	delete[] da;
	delete[] dm;

	//return the trees
	HuffmanTreeNode** res = new HuffmanTreeNode * [2];
	res[0] = mainTree;
	res[1] = distTree;

	return res;
}

#define IBLOCK_END 0x100

//LZ77 tables:

const int LengthExtraBits[] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0 };
const int LengthBase[] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258 };
const int DistanceExtraBits[] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };
const int DistanceBase[] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577 };

//inflate block with LZ77 and huffman
void InflateBlock(BitStream& stream, HuffmanTreeNode* llTree, HuffmanTreeNode* distTree, std::vector<char>& res) {
	i32 tok;

	for (;;) {
		tok = DecodeSymbol(stream, llTree); //get next token
		
		//normal token / byte so append
		if (tok <= 255)
			res.push_back(tok);
		//block end
		else if (tok == 256)
			return;
		//back reference (> 257)
		else {
			i32 lenIdx = tok - 257;

			//get extra length bits and length base
			i32 lenExtraBits = LengthExtraBits[lenIdx],
				len = LengthBase[lenIdx] + stream.readNBits(lenExtraBits);

			//now get distance from distance tree
			i32 distIdx = DecodeSymbol(stream, distTree);

			//compute distance
			i32 distExtraBits = DistanceExtraBits[distIdx],
				dist = DistanceBase[distIdx] + stream.readNBits(distExtraBits);


			//now append characters from back reference
			for (int i = 0; i < len; i++) {
				if (dist >= res.size()) {
					continue;
				}
				res.push_back(res[res.size() - dist]);
				//go back only the distance since as you add nodes the lengths increase 
				//thus causing it to read from the next symbol
			}
		}
	}
}

void InflateBlockDynamic(BitStream& stream, std::vector<char>& res) {
	HuffmanTreeNode** trees = decodeTrees(stream);

#ifdef BALLOON_DEBUG
	std::cout << "\tDECODED TREES" << std::endl;
	PrintTree(trees[0]);
	PrintTree(trees[1]);
#endif

	InflateBlock(stream, trees[0], trees[1], res);
	FreeTree(trees[0]);
	FreeTree(trees[1]);
	delete[] trees;
}

template<typename _Ty> void memsetbutbetter(_Ty* dest, _Ty val, size_t sz) {
	for (i32 i = 0; i < sz; i++)
		dest[i] = val;
}


void InflateBlockStatic(BitStream& stream, std::vector<char>& res) {
	//static bit lists
	u32* bitList = new u32[288];

	memsetbutbetter(bitList, (unsigned)8, 144);
	memsetbutbetter(bitList + 144, (unsigned)9, 112);
	memsetbutbetter(bitList + 256, (unsigned)7, 24);
	memsetbutbetter(bitList + 280, (unsigned)8, 8);

	HuffmanTreeNode* llTree = BitLengthsToHTree(bitList, 288, (unsigned*)CreateAlphabet(286), 286); //main tree

	//distance tree
	delete[] bitList;
	bitList = new u32[30];

	memsetbutbetter(bitList, (unsigned)5, 30);

	HuffmanTreeNode* distTree = BitLengthsToHTree(bitList, 30, (unsigned*)CreateAlphabet(30), 30);

	//inflate le data
	InflateBlock(stream, llTree, distTree, res);

	delete[] bitList, llTree, distTree;
}

void InflateBlockNone(BitStream& stream, std::vector<char>& res) {
	i32 blockSize = stream.readValue(2);
	i32 nBlockSize = stream.readValue(2);
	for (i32 i = 0; i < blockSize; i++)
		res.push_back(stream.readByte());
}

struct BData {
	char* bytes;
	size_t len;
};

BData* _Inflate(BitStream& stream) {
	bool eos = false;

	std::vector<char> res;

	while (!eos) {


		i32 bFinal = stream.readBit();
		i32 bType = stream.readNBits(2);

		switch (bType) {
		case 0: {
			InflateBlockNone(stream, res);
			break;
		}
		case 1: {
			InflateBlockStatic(stream, res);
			break;
		}
		case 2: {
			InflateBlockDynamic(stream, res);
			break;
		}
		default: {
			std::cout << "Error invalid block! Type: " << bType << std::endl;
		}
		}

		eos = bFinal;
	}

	BData* _res = new BData;

	_res->bytes = new char[res.size()];
	ZeroMem(_res->bytes, res.size());
	memcpy(_res->bytes, res.data(), res.size());
	_res->len = res.size();

	return _res;
}

ZResult* Zlib::Inflate(u32* bytes, size_t len) {
	BitStream stream = BitStream(bytes, len);
	i32 compressionInfo = stream.readByte();
	//check copression mode
	i32 compressionMode = compressionInfo & 0xf;
	assert(compressionMode == 8);


	//copmression flags
	i32 compressionFlags = stream.readByte();

	//check sum for header info
	assert((compressionInfo * 256 + compressionFlags) % 31 == 0);


	//check for dictionary
	bool dictionary = (compressionFlags >> 5) & 0x1;

	if (dictionary)
		return nullptr; //no dictionary support yet :/

	ZResult* res = new ZResult;

	res->compressionLevel = (compressionFlags >> 6) & 0b11;
	res->compressionMode = compressionMode;


#ifdef BALLOON_DEBUG
	std::cout << "Compression Level: " << res->compressionLevel << "\nCompression Info: " << compressionInfo << "\nMode: " << compressionMode << std::endl;
#endif
	BData* iDat = _Inflate(stream);

	res->bytes = new u32[iDat->len];
	for (i32 i = 0; i < iDat->len; i++) {
		res->bytes[i] = (u32)(unsigned char)iDat->bytes[i];
	}
	delete[] iDat->bytes;
	res->len = iDat->len;
	delete iDat;

	return res;
};

std::vector<HuffmanTreeNode> _SortVector(std::vector<HuffmanTreeNode>& _vec) {
	std::sort(_vec.begin(), _vec.end(), [](HuffmanTreeNode& a, HuffmanTreeNode& b) {

		/*if (a.count == b.count) {
			if (a.branch) return false;

			return true;
		}*/

		//if (a.count == b.count) {
			//return a.val > b.val;
		//}

		return a.count < b.count;
		});

	return _vec;
}

//get char count
u32* GetCharCount(u32* bytes, size_t len, size_t alphabetSize) {
	u32* _charCount = new u32[alphabetSize]; //allocate memory for the counts

	ZeroMem(_charCount, alphabetSize); //zeromemory

	for (i32 i = 0; i < len; i++) {
		if (INRANGE(bytes[i], 0, alphabetSize)) {
			_charCount[bytes[i]]++;
		}
		else
			std::cout << "Error, unknown symbol: " << bytes[i] << " | Not in range: " << alphabetSize << std::endl;
	}

	return _charCount;
}

#include <queue>
#include <map>

#define SIGN(v) (v >= 0 ? 1 : -1)
#define MAX(a,b) (a >= b ? a : b)

//keep track of each node's depth with 0 beign the deepest and calculating a 
//new nodes depth with max(left, right) + 1 and when sorting use: #define smaller(tree, n, m, depth) (tree[n].Freq < tree[m].Freq || (tree[n].Freq == tree[m].Freq && depth[n] <= depth[m]))

struct treeComparison {
	bool operator()(HuffmanTreeNode* a, HuffmanTreeNode* b) {
		if (a->count == b->count)
			return a->depth > b->depth;
		return a->count > b->count;
	}
};

//TODO try priority queue
HuffmanTreeNode* GenerateBaseTree(u32* _charCount, size_t alphabetSize) {
	HuffmanTreeNode* root = nullptr;

	//
	std::priority_queue<HuffmanTreeNode*, std::vector<HuffmanTreeNode*>, treeComparison> tNodes;

	//convert char counts to temp tree vector
	for (size_t i = 0; i < alphabetSize; i++) {
		if (_charCount[i] <= 0) continue;

		HuffmanTreeNode* _node = new HuffmanTreeNode;

		_node->count = _charCount[i];
		_node->val = i;

		tNodes.push(_node);
	}

	if (tNodes.size() <= 0)
		return nullptr;

	//HuffmanTreeNode* root = nullptr;

	while (tNodes.size() > 1) {
		HuffmanTreeNode* newNode = new HuffmanTreeNode;

		HuffmanTreeNode* left = tNodes.top();
		tNodes.pop();

		HuffmanTreeNode* right = tNodes.top();
		tNodes.pop();

		newNode->left = left;
		newNode->right = right;
		newNode->val = -1;

		newNode->depth = MAX(left->depth, right->depth) + 1;

		newNode->count = left->count + right->count;

		root = newNode;

		tNodes.push(newNode);
	}


	HuffmanTreeNode* fNode = tNodes.top();

	if (!root)
		root = fNode;

	return root;
}

/**
 *
 * getTreeBitLens
 *
 * Get the bit lengths of each symbol in a huffman tree give
 * the tree.
 *
 * This function calls itself recursivley
 *
 * Params:
 *
 *	tree -> a pointer to the root node in a huffman tree
 *	alphabetSize -> number of characters that the tree can hold (normally 288)
 *
 *  currentLen and cbl should not be set
 *
 */

u32* getTreeBitLens(HuffmanTreeNode* tree, size_t alphabetSize, i32 currentLen = 0, u32* cbl = nullptr) {
	if (cbl == nullptr) {
		cbl = new u32[alphabetSize];
		memset(cbl, 0, sizeof(u32) * alphabetSize);
	}

	if (tree->right != nullptr || tree->left != nullptr) {
		if (tree->left != nullptr) getTreeBitLens(tree->left, alphabetSize, currentLen + 1, cbl);
		if (tree->right != nullptr) getTreeBitLens(tree->right, alphabetSize, currentLen + 1, cbl);
	}
	else {
		if (tree->val < alphabetSize)
			cbl[tree->val] = currentLen > 0 ? currentLen : 1;
		else
			std::cout << "Invalid bl_symbol [" << tree->val << "]" << "  " << alphabetSize << std::endl;
	}

	return cbl;
}
/*

Function to convert a huffman tree into a canonical huffman tree
via BitLengthsToHTree

Params:

	tree -> pointer to root node of a Huffman tree
	alphabetSize -> number of characters that the tree can hold (normally 288)

*/

HuffmanTreeNode* CovnertTreeToCanonical(HuffmanTreeNode* tree, size_t alphabetSize, bool deleteOld = false) {
	//get bit lengths of each code
	u32* bitLens = getTreeBitLens(tree, alphabetSize);

#ifdef BALLOON_DEBUG
	std::cout << "BitLen: ";
	for (int i = 0; i < alphabetSize; i++)
		if (bitLens[i] > 0) std::cout << bitLens[i];

	std::cout << std::endl;

	std::cout << "BitDiffs: " << std::endl;

	for (int i = 0; i < alphabetSize; i++) {
		if (bitLens[i] > 0 && i < _treeLens.size() && bitLens[i] != _treeLens[i]) {
			std::cout << (char)i << "  " << i << "   " << bitLens[i] << "  " << _treeLens[i] << std::endl;
		}
	}

	std::cout << "BitLens: " << std::endl;
	for (int i = 0; i < alphabetSize; i++)
		if (bitLens[i] > 0) std::cout << "BitLen: " << bitLens[i] << " " << i << " " << (char)i << std::endl;

	std::cout << std::endl;
#endif

	//start to create le tree
	HuffmanTreeNode* cTree = BitLengthsToHTree(bitLens, alphabetSize, (u32*)CreateAlphabet(alphabetSize), alphabetSize);

	//free memory and return new tree
	delete[] bitLens;

	if (deleteOld)
		delete[] tree;

	return cTree;
}

#define LZ77_MIN_MATCH 3 // min match size for back reference
#define LZ77_MAX_MATCH 258

//from zlib on github
const int compression_table_old[10][5] = {
	/*      good lazy nice max_chain_length */
	/* 0 */ {0,    0,  0,    0},    /* store only */
	/* 1 */ {4,    4,  8,    4},    /* max speed, no lazy matches */
	/* 2 */ {4,    5, 16,    8},
	/* 3 */ {4,    6, 32,   32},
	/* 4 */ {4,    4, 16,   16},    /* lazy matches */
	/* 5 */ {8,   16, 32,   32},
	/* 6 */ {8,   16, 128, 128},
	/* 7 */ {8,   32, 128, 256},
	/* 8 */ {32, 128, 258, 1024},
	/* 9 */ {32, 258, 258, 4096}    /* max compression */
};


void printb(char* buf, size_t sz) {
	for (int i = 0; i < sz; i++) {
		if (buf[i] != 0)
			std::cout << buf[i];

	}

	std::cout << '\n';
}

void printWindow(char* win, size_t winSz, i32 readChar) {
	for (int i = 0; i < winSz; i++) {
		if (win[i] != 0)
			std::cout << win[i];
		else
			std::cout << " ";
	}

	std::cout << std::endl;

	for (int i = 0; i < winSz; i++) {
		if (i != readChar)
			std::cout << " ";
		else
			std::cout << "^";
	}

	std::cout << std::endl;
}

//get codes and indexs from length
const i32 nLengths = 29;
const i32 nDists = 30;

//LengthBase
//LengthExtraBits
i32 lz77_get_len_idx(size_t len) {
	i32 lastIdx = 0;

	for (i32 i = 0; i < nLengths; i++) {
		if (LengthBase[i] > len) break;

		lastIdx = i;
	}

	return lastIdx;
}

//get codes and indexs from distances

//LengthBase
//LengthExtraBits
i32 lz77_get_dist_idx(size_t dist) {
	i32 lastIdx = 0;

	for (i32 i = 0; i < nLengths; i++) {
		if (DistanceBase[i] > dist) break;

		lastIdx = i;
	}

	return lastIdx;
}
#include <unordered_map>

#define MIN(a,b) (a > b ? b : a)
#define MAX_MATCH 258

void shiftTable(std::unordered_map<std::string, i32>& table, const size_t amount) {
	for (auto& item : table) {
		item.second -= amount;

		if (item.second <= 0)
			table.erase(table.find(item.first));
	}
}

#define INSERT_HASH(hash, key, value, res) res = hash[key]; hash[key] = value+1;

i32 longest_match(u32* _win, u32* sWin, u32* wEnd, i32 maxMatch) {
	i32 sz = 1;
	u32* match = sWin, * win = _win;

	if (match == win) return 1;

	do {} while (
		*win++ == *match++ &&
		win < wEnd &&
		++sz < maxMatch
	);

	return sz - 1;
}

#define MAX(a, b) (a > b ? a : b)

void flushHash(std::unordered_map<std::string, i64>& table, u64 offset) {
	std::vector<std::string> eraseStr;
	for (auto& [key, value] : table) {
		if ((i64)(value - offset) <= 0)
			eraseStr.push_back(key);
	}

	for (auto& str : eraseStr)
		table.erase(str);
}

void WriteVBitsToStream(BitStream& stream, const long val, size_t nBits) {
	if (nBits <= 0) return;

	for (i32 i = 0; i < nBits; i++)
		stream.writeBit((val >> i) & 1);
}

void WriteVPBitsToStream(BitStream& stream, const long val, size_t nBits) {
	if (nBits <= 0) return;

	for (i32 i = 0; i < nBits; i++)
		stream.writeBit((val >> i) & 1);
}

void WriteRVBitsToStream(BitStream& stream, const long val, size_t nBits) {
	if (nBits <= 0) return;

	for (i32 i = 0; i < nBits; i++)
		stream.writeBit((val >> ((nBits - 1) - i)) & 1);
}

//function to finish lz77 encoding
void GenerateLz77Stream(BitStream& rStream, std::vector<u32>& res, HuffmanTreeNode* distTree) {
	//now write stuff to the stream
	for (const auto& lByte : res) {
		//check for backreference
		if (lByte >= 257) {
			//extract individual values from back reference
			i32 lenIdx = (lByte & 0xfff) - 257,
				distIdx = (lByte >> 0xc) & 0xf,
				lenExtra = (lByte >> 0x10) & 0xf,
				distExtra = (lByte >> 0x14) & 0xf;

			//construct back reference as bytes
			rStream.writeValue(lenIdx);

			WriteVBitsToStream(rStream, lenExtra, 3);

			//encode and add distance index symbol
			rStream.writeValue(EncodeSymbol(distIdx, distTree));
			WriteVBitsToStream(rStream, distExtra, 3);
		}
		//write current byte if no back reference
		else if (lByte < 256) {
			rStream.writeValue(lByte);
		}
	}
}

/*

LZ77 encode

This is the fast version of le algorithm.

Rn it can only get around 4-6 mb/s but thats better than 3kb/s


*/

//#define BALLOON_DEBUG

struct lz77_res {
	std::vector<u32> rStream;
	u32* charCounts;
	HuffmanTreeNode* distTree;
};

lz77_res* lz77_encode(u32* bytes, size_t sz, const u32 winBits, const size_t lookahead, HuffmanTreeNode* _distTree = nullptr, const int alphabetSize = 288) {
	//compute lookahead size
	const size_t winSz = MAX((size_t)1 << winBits, lookahead + 1);

	//result
	lz77_res* res = new lz77_res;

	//get the char counts of the encoded data
	res->charCounts = new u32[alphabetSize];
	ZeroMem(res->charCounts, alphabetSize);

	//get pointer to window
	u32* window = bytes;
	u32* end = window + sz;
	//memset(window, 0, winSz);
	const size_t readPos = winSz - lookahead;
	size_t currentByte = 0;

	//hash table and output stream
	std::unordered_map<std::string, i64> hashTable;
	std::vector<u32> rStream;

	//hsing info stuff
	std::string insertStr;
	i64 prevHash = 0;
	size_t shiftAmount = 0;
	u64 hashOffset = 0;

	u32 fsz = 0; //final size

	//debug
	i32 printThreshold = sz / (1 << 4);

	//distance and length info
	std::vector<i32> matches, lmatches, distanceIdxs;

	u32* distCounts = new u32[nDists];
	ZeroMem(distCounts, nDists);

	//res->rStream.push_back(*window);
	//res->charCounts[*window]++;

	//do some lz77 encoding here
	do {
#ifdef BALLOON_DEBUG
		if (printThreshold != 0 && currentByte % printThreshold == 0 || true) {
			//std::cout << "Compressed: [" << currentByte << " / " << sz << "] \n";
			//std::cout << "Map Sz: " << hashTable.size() << "\n";
		}
#endif

		fsz++;


		//win shift but it's actually fast
		window += shiftAmount; //goofy pointer manipulation
		if (window >= end) break;
		currentByte += shiftAmount;

		hashOffset += shiftAmount; //shift hash
		shiftAmount = 1;

		//hash flushing
		if (hashOffset % winSz == 0 && hashOffset > 1)
			flushHash(hashTable, hashOffset);
		//
		if (*window >= alphabetSize || window > (bytes + sz)) {
			std::cout << "[Warning] window overflow or invalid char!" << std::endl;
			if (window > bytes + sz) break;
			continue;
		} //simple little check

		//add hash to table
		prevHash = 0;
		insertStr = ((char)*window + (char)*(window + 1) + (char)*(window + 2));

		//set new hash
		i64* htv = &hashTable[insertStr];
		prevHash = *htv;
		*htv = currentByte;

		//prevHash -= hashOffset;

		if (prevHash > 0) {
			size_t _bestMatch = longest_match(bytes + prevHash, window, bytes + sz, MAX_MATCH);
			if (_bestMatch <= 0) _bestMatch = 1;
			if (_bestMatch <= 3) {
				res->rStream.push_back(*window);
				res->charCounts[*window]++;
				continue;
			}

			//compute distance
			i32 dist = (size_t)(window - (bytes + prevHash)),
				matchLen = _bestMatch;


			//add back reference
			i32 lenIdx = lz77_get_len_idx(_bestMatch);

			//get distance match
			i32 distIdx = lz77_get_dist_idx(dist);

			distanceIdxs.push_back(res->rStream.size()); //add distance index
			distCounts[distIdx]++; //increase distance count for le distance tree

			//output back reference
			res->rStream.push_back(257 + lenIdx);
			res->rStream.push_back(distIdx);
			res->rStream.push_back(_bestMatch - LengthBase[lenIdx]);
			res->rStream.push_back(dist - DistanceBase[distIdx]);

			res->charCounts[257 + lenIdx]++; //increase char count

			//some stuff to prepare for next itneration
			shiftAmount = _bestMatch;
			fsz += 2;
			continue;
		}
		//else just add character
		else {
			res->charCounts[*window]++;
			res->rStream.push_back(*window);
		}

#ifdef BALLOON_DEBUG
		if (currentByte % printThreshold == 0)
			std::cout << "Compressed: [" << currentByte << " / " << sz << "]" << std::endl;
#endif
	} while (currentByte < sz);

	res->distTree = _distTree;

	//create distance tree
	if (_distTree == nullptr) {
		HuffmanTreeNode* bdTree = GenerateBaseTree(distCounts, nDists);

		if (bdTree != nullptr) {
			res->distTree = CovnertTreeToCanonical(bdTree, nDists);
			FreeTree(bdTree);
			//distance tree codes
			GenerateCodeTable(res->distTree, 30);
		}
		else {
			res->distTree = new HuffmanTreeNode;
			res->distTree->alphabetSz = 30;
			res->distTree->val = 1;
			res->distTree->count = 1;
		}
	}

#ifdef BALLOON_DEBUG
	std::cout << "Compression Ratio: " << (float)fsz / (float)sz << std::endl;
#endif

	delete[] distCounts;

	return res;
}

#define DEFAULT_ALPHABET_SZ 288

//function to remove the zeros at the end of an array
template<typename _T> arr_container<_T>* clipArr(_T* dat, size_t _sz) {
	const _T* end = dat + _sz;

	_T* cur = dat;
	_T* last = nullptr;

	do {
		if (*cur != 0)
			last = cur;
	} while (++cur < end);

	if (last == nullptr) {
		arr_container<_T>* defRes = new arr_container<_T>;
		defRes->dat = nullptr;
		defRes->sz = 0;
		return defRes;
	}

	size_t sz = (size_t)(last - dat) + 1;
	arr_container<_T>* res = new arr_container<_T>;
	res->dat = new _T[_sz];
	ZeroMem(res->dat, _sz);
	memcpy(res->dat, dat, sizeof(_T) * sz);
	res->sz = sz;
	//delete[] dat;
	return res;
}

const uint32_t MOD_ADLER = 65521; //0xfff1

#define ADLER32_BASE 1

void computeAdler32CheckSum(u32& current, byte dat) {
	u32 a = current & 0xffff;
	u32 b = (current >> 16) & 0xffff;

	a = (a + dat) % MOD_ADLER;
	b = (b + a) % MOD_ADLER;

	current = (b << 16) | a;
}

#define RLE_Z2_BASE 11
#define RLE_Z1_BASE 3

#define RLE_Z2_MASK 0x7f
#define RLE_Z1_MASK 0x7

#define RLE_L_BITS 0x2
#define RLE_Z1_BITS 0x3
#define RLE_Z2_BITS 0x7

#define RLE_L_MASK 0x3
#define RLE_L_BASE 0x3

/*
Function to do the basic rle encoding for the code length tree
*/
arr_container<u32> rleEncode(u32* dat, size_t sz) {
	std::vector<u32> rleDat; //result

	//so ini vars
	u32* cur = dat, *end = dat + sz; //current and end pointers
	size_t cpySz = 1; //size of longest copy
	size_t tSz = 0;

	//encoding loop
	do {
		//get size of longest match
		u32* temp = cur;
		size_t _tMask = *cur == 0 ? RLE_Z2_MASK : RLE_L_MASK;
		do {} while (*++temp == *cur && temp < end && (size_t)(temp - cur) < _tMask);
		cpySz = (size_t)(temp - cur);
		//DEBUG REMINDER: the encoding length is trying to ecnode one but is getting 4 because its 1 + base of 3
		switch (*cur) {
			//0 repetition
		case 0: {
			//if we have enough for a back reference then create one
			if (cpySz >= RLE_Z1_BASE) {
				//18
				if (cpySz >= RLE_Z2_BASE) {
					rleDat.push_back(18);
					rleDat.push_back((cpySz - RLE_Z2_BASE) & RLE_Z2_MASK);
#ifdef BALLOON_DEBUG
					std::cout << "SSZ: " << ((cpySz - RLE_Z2_BASE) & RLE_Z2_MASK) << " " << cpySz << std::endl;
#endif
				}
				//17
				else {
					rleDat.push_back(17);
					rleDat.push_back((cpySz - RLE_Z1_BASE) & RLE_Z1_MASK);
				}
			}
			//else well just add all the zeros
			else {
				for (i32 i = 0; i < cpySz; i++)
					rleDat.push_back(0);
			}
			break;
		}
			  //other characters that arent zero
		default: {
			//if we have enough for a back reference then create one
			if (cpySz >= RLE_L_BASE + 1) {
				rleDat.push_back(*cur);
				rleDat.push_back(16);
				rleDat.push_back((cpySz - RLE_L_BASE - 1) & RLE_L_MASK);
			}
			//else well just add all the zeros
			else {
				for (i32 i = 0; i < cpySz; i++)
					rleDat.push_back(*cur);
			}
			break;
		}
		}

		tSz += cpySz;
	} while ((cur += cpySz) < end);

	if (tSz != sz)
		std::cout << "Rle Error :O" << std::endl;

#ifdef BALLOON_DEBUG
	std::cout << "final size: " << tSz << std::endl;
#endif

	//return result
	arr_container<u32> res;
	res.sz = rleDat.size();
	res.dat = new u32[res.sz];
	memcpy(res.dat, rleDat.data(), sizeof(u32) * res.sz);
	return res;
}

u32* GetRleCharCounts(arr_container<u32> dat, size_t alpha_sz) {
	assert(dat.sz > 0);
	u32* res = new u32[alpha_sz];
	ZeroMem(res, alpha_sz);
	for (size_t i = 0; i < dat.sz; i++) {
		if (!INRANGE(dat.dat[i], 0, alpha_sz)) continue;
		res[dat.dat[i]]++;
		if (dat.dat[i] > 15) i++;
	}
	return res;
}

unsigned long bitReverse(unsigned long v, size_t nBits) {
	unsigned long rs = 0x00;
	for (i32 i = nBits - 1; i >= 0; i--)
		rs |= (((v & (1 << i)) >> i) & 1) << ((nBits - 1) - i); //yes just a lot of bit shifting stuff
	return rs;
}

//function to write the trees to a deflate stream
//this crust is the bane of my existence
/*

Header:

5 bits -> number of tree codes - 257
5 bits -> numner of distance codes - 1
4 bits -> number of codes in codelength tree - 4

Info for code length tree:

19 values in code length alphabet

0-15 -> code length values
16 -> repeat last code length 3 - 6 times
	2 bits + 3
17 -> repeast 0 3-10 times
	3 bits + 3
18 -> repeat 0 11-138 times
	7 bits + 11

Code Length Tree Encoding:

3 bits * nCodesInTree -> tree data

Encode next to trees with the above tree starting with the main tree and then the distance tree
	* for the distnace if there are no distances just create a 1 node tree to replace it's spot incase having no tree causes errors somewhere

*/
void writeTrees(BitStream& stream, HuffmanTreeNode* litTree, HuffmanTreeNode* distTree) {
	//ok im just rewrite this but actually decent
	//so uh first thing first, create the code length tree
	/*

	First Step:
		run length encode the data first so then we can construct the code length tree

	*/
	//PrintTree(litTree);
	//PrintTree(distTree);
	//std::cout << "Alphabet Sizes: " << litTree->alphabetSz << " " << distTree->alphabetSz << std::endl;
	u32* uc_lt_bitLens = getTreeBitLens(litTree, litTree->alphabetSz); //get literal tree bit lengths
	u32* uc_dt_bitLens = getTreeBitLens(distTree, distTree->alphabetSz); //get dist tree bit lengths

	arr_container<u32>
		lt_bitLens = arr_container<u32>(uc_lt_bitLens, litTree->alphabetSz),
		dt_bitLens = arr_container<u32>(uc_dt_bitLens, distTree->alphabetSz);

	delete[] uc_lt_bitLens, uc_dt_bitLens;
	//combine the bit lenghts
	size_t cbSz = lt_bitLens.sz + dt_bitLens.sz;
	u32* combinedBitLens = new u32[cbSz];

	memcpy(combinedBitLens, lt_bitLens.dat, sizeof(u32) * lt_bitLens.sz);
	memcpy(combinedBitLens + lt_bitLens.sz, dt_bitLens.dat, sizeof(u32) * dt_bitLens.sz);

#ifdef BALLOON_DEBUG
	std::cout << "Pre Combined Bitlens: ";

	for (int i = 0; i < cbSz; i++)
		std::cout << combinedBitLens[i] << " " << i;

	std::cout << std::endl;
#endif
		 
	//now rle encode
	arr_container<u32> enc_dat = rleEncode(combinedBitLens, cbSz);

#ifdef BALLOON_DEBUG
	for (i32 i = 0; i < enc_dat.sz; i++) {
		std::cout << "cl sym: " << enc_dat.dat[i] << std::endl;
		if (enc_dat.dat[i] > 15) i++;
	}
#endif

	//now to create the tree
	const size_t clAlphaSz = 19;
	u32* clCCounts = GetRleCharCounts(enc_dat, clAlphaSz);

	HuffmanTreeNode* tempTree = GenerateBaseTree(clCCounts, clAlphaSz);
	HuffmanTreeNode* clTree = CovnertTreeToCanonical(tempTree, clAlphaSz, false); //this is the code length tree
	clTree->alphabetSz = clAlphaSz;

#ifdef BALLOON_DEBUG
	std::cout << "Le Code Length Tree" << std::endl;
	PrintTree(clTree);
#endif

	u32* clBitlens = getTreeBitLens(clTree, clAlphaSz);

	i32 HLIT = litTree->alphabetSz - 257,
		HDIST = distTree->alphabetSz - 1,
		HCL = clTree->alphabetSz - 4;


	//write header
	WriteVBitsToStream(stream, HLIT, 5);
	WriteVBitsToStream(stream, HDIST, 5);
	WriteVBitsToStream(stream, HCL, 4);

	//write the cltree
	for (i32 i = 0; i < clAlphaSz; i++) {
		WriteVBitsToStream(
			stream,
			clBitlens[CodeLengthCodesOrder[i]],
			3
		);
	}

	//now encode all the data (this isnt working :/)
	for (i32 i = 0; i < enc_dat.sz; i++) {
#ifdef BALLOON_DEBUG
		std::cout << "Ecoding CL Symbol: " << (int)enc_dat.dat[i] << " [" << i << "/" << enc_dat.sz << "]\n";
#endif
		if (enc_dat.dat[i] < 16) {
			//just put value if not backreference
			WriteVBitsToStream(stream,
				bitReverse(EncodeSymbol(
					enc_dat.dat[i],
					clTree
				), clBitlens[enc_dat.dat[i]]),
				clBitlens[enc_dat.dat[i]]
			);
			//16-18 back reference
		}
		else {
			u32 sym = enc_dat.dat[i];

			WriteVBitsToStream(stream,
				bitReverse(EncodeSymbol(
					sym,
					clTree
				), clBitlens[sym]),
				clBitlens[sym]
			);

			//now encode back reference lengths
			switch (sym) {
			case 16: {
				WriteVBitsToStream(
					stream,
					enc_dat.dat[++i],
					RLE_L_BITS
				);
				break;
			}
			case 17: {
				WriteVBitsToStream(
					stream,
					enc_dat.dat[++i],
					RLE_Z1_BITS
				);
				break;
			}
			case 18: {
				WriteVBitsToStream(
					stream,
					enc_dat.dat[++i],
					RLE_Z2_BITS
				);
				break;
			}
			}
#ifdef BALLOON_DEBUG
			std::cout << "[" << enc_dat.dat[i] << "]";
#endif
		}
	}
#ifdef BALLOON_DEBUG
	std::cout << std::endl;
#endif

	//memory management
	FreeTree(tempTree);
	FreeTree(clTree);

	delete[] clBitlens, clCCounts, combinedBitLens, enc_dat.dat/*, lt_bitLens->dat, dt_bitLens->dat*/;
	//delete lt_bitLens, dt_bitLens;
}

//specialized version of get char count for a lz77 stream and also filters the stream
u32* getLzCharCounts(lz77_res* s, size_t alphabetSz) {
	i32 i = 0, sz = s->rStream.size(), _c = NULL;
	u32* counts = new u32[alphabetSz];
	ZeroMem(counts, alphabetSz);
	std::vector<u32> nStream;
	do {
		_c = s->rStream[i];
		if (_c > 0 && _c < alphabetSz) {
			counts[_c]++;
			nStream.push_back(_c);
			if (_c > 257)
				i += 3;
		}
		else {
			std::cout << "INVALID CHAR: " << _c << std::endl;
		}
	} while (++i < sz);

	s->rStream = nStream;

	return counts;
}

/**
 *
 * Function to add deflate blocks deflate blocks
 *
 */

void GenerateDeflateBlock(BitStream& stream, u32* bytes, size_t len, const size_t winBits, const int level, bool finalBlock = false) {
	const int bFinal = (byte)finalBlock & 0x01;
	int bType = 0;

	//dynamic block
	if (level > 0)
		bType = 2;

	//generate block

	//write block header
	stream.writeBit(bFinal);
	stream.writeBit((bType & 0b01));
	stream.writeBit((bType & 0b10) >> 1);

	//now compress the data
	switch (level) {
		//No compression
	case 0: {
		i32 i = len;
		stream.writeValue((i32)len);
		stream.writeValue((i32)len);
		while ((int)--i > 0)
			stream.writeValue(bytes[i]);
		break;
	}
		  //Huffman Only
	case 1: {
		u32* charCounts = GetCharCount(bytes, len, DEFAULT_ALPHABET_SZ);

		HuffmanTreeNode* baseTree = GenerateBaseTree(charCounts, DEFAULT_ALPHABET_SZ);
		HuffmanTreeNode* tree = CovnertTreeToCanonical(baseTree, DEFAULT_ALPHABET_SZ);
		tree->alphabetSz = DEFAULT_ALPHABET_SZ;
		HuffmanTreeNode* distTree = new HuffmanTreeNode;
		u32* treeCodeLengths = getTreeBitLens(tree, DEFAULT_ALPHABET_SZ);

		//create a null node this will just get discarded at some point during inflate
		distTree->alphabetSz = 30;
		distTree->count = 1;
		distTree->val = NULL;

#ifdef BALLOON_DEBUG
		//debug ;;-;;
		std::cout << "Lit Tree: " << std::endl;
		PrintTree(tree);

		std::cout << "---------------------------" << std::endl;

		PrintTree(distTree);
#endif

		//write the trees
		writeTrees(stream, tree, distTree);

		u32 checkSum = ADLER32_BASE;

		//now write all the bytes
		for (i32 i = 0; i < len; i++) {
			//check sum
			computeAdler32CheckSum(checkSum, bytes[i]);

#ifdef BALLOON_DEBUG
			//continue to write stuff
			std::cout << "Symbol: " << " " << (char)bytes[i] << " " << EncodeSymbol(bytes[i], tree) << "   " << treeCodeLengths[bytes[i]] << std::endl;
#endif
			WriteVPBitsToStream(
				stream,
				bitReverse(
					EncodeSymbol(bytes[i], tree),
					treeCodeLengths[bytes[i]]
				),
				treeCodeLengths[bytes[i]]
			);
		}

#ifdef BALLOON_DEBUG
		std::cout << "Deflate Data: " << std::endl << "--------------" << std::endl;

		for (i32 i = 0; i < stream.sz; i++) {
			std::cout << std::hex << stream.bytes[i] << " ";
		}

		std::cout << std::dec << std::endl << "-----------------------" << std::endl;
#endif

		//add check sum
		stream.writeValue(checkSum);

		//memory clean up
		FreeTree(tree);
		FreeTree(distTree);
		FreeTree(baseTree);
		delete[] treeCodeLengths, charCounts;

		//we done :D
		break;
	}
		  //lz77 and huffman
	case 2: {
		lz77_res* lzr = lz77_encode(bytes, len, winBits, 258);

#ifdef BALLOON_DEBUG
		std::cout << "LZ77 ENCODED DAT: ";
		for (auto u : lzr->rStream)
			std::cout << (char)u << " ";
		std::cout << std::endl;

		std::cout << "LZ77 ENCODED DAT u32: ";
		for (auto u : lzr->rStream)
			std::cout << (u32)u << " ";
		std::cout << std::endl;
#endif

		u32* charCounts = lzr->charCounts;

#ifdef BALLOON_DEBUG
		for (auto b : lzr->rStream) {
			std::cout << b << " ";
		}

		std::cout << std::endl;

		for (int i = 0; i < DEFAULT_ALPHABET_SZ; i++) {
			std::cout << "Char Count: " << charCounts[i] << " " << i << std::endl;
		}
#endif


		HuffmanTreeNode* baseTree = GenerateBaseTree(charCounts, DEFAULT_ALPHABET_SZ);
		HuffmanTreeNode* tree = CovnertTreeToCanonical(baseTree, DEFAULT_ALPHABET_SZ);
		tree->alphabetSz = DEFAULT_ALPHABET_SIZE;

#ifdef BALLOON_DEBUG
		std::cout << "tree :D" << std::endl;
		PrintTree(tree);

		std::cout << "---------------------------" << std::endl;

		PrintTree(lzr->distTree);
#endif

		u32* treeCodeLengths = getTreeBitLens(tree, DEFAULT_ALPHABET_SZ);
		u32* distCodeLengths = getTreeBitLens(lzr->distTree, DEFAULT_ALPHABET_SZ);

		//write the trees
		writeTrees(stream, tree, lzr->distTree);

		u32 checkSum = ADLER32_BASE;

		//now write all the bytes
		for (i32 i = 0; i < lzr->rStream.size(); i++) {
			u32 sym = lzr->rStream[i];
			//check sum
			computeAdler32CheckSum(checkSum, bytes[i]);

			if (sym >= 257) {

				//continue to write stuff
				//length base
				WriteVBitsToStream(
					stream,
					bitReverse(
						EncodeSymbol(sym & 0xfff, tree),
						treeCodeLengths[sym]
					),
					treeCodeLengths[sym]
				);

				//back reference
				i32 distIdx = lzr->rStream[++i],
					lengthExtra = lzr->rStream[++i],
					distExtra = lzr->rStream[++i];

				//length extra
				WriteVBitsToStream(
					stream,
					lengthExtra,
					LengthExtraBits[(sym)-257]
				);

				//distance base
				WriteVBitsToStream(
					stream,
					bitReverse(
						EncodeSymbol(distIdx, lzr->distTree),
						distCodeLengths[distIdx]
					),
					distCodeLengths[distIdx]
				);

				//distance extra
				WriteVBitsToStream(
					stream,
					distExtra,
					DistanceExtraBits[distIdx]
				);
			}
			else {
				//continue to write stuff
				WriteVBitsToStream(
					stream,
					bitReverse(
						EncodeSymbol(sym, tree),
						treeCodeLengths[sym]
					),
					treeCodeLengths[sym]
				);
			}
		}

		//add check sum
		stream.writeValue(checkSum);

		//memory management
		delete[] treeCodeLengths, charCounts;
		FreeTree(baseTree);
		FreeTree(tree);
		//and we done :D again ;-;
		break;
	}
	default: {
		break;
	}
	}
}

/**
 *
 *  DEFLATE
 *
 * This is the main function for deflate called with Zlib::Deflate.
 *
 * This function takes some bytes and applies huffman coding and LZ77
 * to compress the data.
 *
 * Idk what else to put here to sound fancy
 *
 */

ZResult Zlib::Deflate(u32* bytes, size_t len, const size_t winBits, const int level) {
	ZResult _znull;
	if (bytes == nullptr || len <= 0) return _znull; //quick length check

	//compress the data first
	BitStream rStream = BitStream(0xff);

	if (winBits > 15)
		return _znull;

	//generate some of the fields
	byte cmf = (0x08) | (((winBits - 8) & 0b1111) << 4);
	byte flg = 0; //all the data is just going to be 0

	flg = 31 - (cmf * 256) % 31; //to make sure the check sum is good

#ifdef BALLOON_DEBUG
	std::cout << "Compression Info: " << (i32)cmf << " flg: " << (i32)flg << std::endl;
#endif
	//write the header info
	rStream.writeValue(cmf);
	rStream.writeValue(flg);

	u32* nBytes = new u32[len + 1];
	memcpy(nBytes, bytes, len * sizeof(u32));
	nBytes[len] = 0x100;
	GenerateDeflateBlock(rStream, nBytes, len + 1, winBits, level, true);

	delete[] nBytes;

	ZResult res;

	//rStream.clip();

	res.bytes = rStream.bytes;
	res.len = rStream.sz;

#ifdef BALLOON_DEBUG
	std::cout << "Out Len: " << res.len << "  In Len: " << len << " " << rStream.asz << std::endl;
#endif

	res.compressionLevel = level;
	res.checkSum = 0xffffffff;

	return res;
}


//debug function don't call unless testing, wait, why is this here

#ifdef BALLOON_DEBUG
void Huffman::DebugMain() {

	//string search testing
	char testBuf[] = "This is some test text that will be deflate then reinflate. ABABAB ABABABAB       <- test backreference";

	size_t len = strlen(testBuf);

	u32* uTestStr = new u32[len];

	srand(time(NULL));

	for (size_t i = 0; i < len; i++)
		uTestStr[i] = testBuf[i];

	const char* lz77_enc_test = "This is some test text that will be deflate then reinflate. ABABAB ABABABAB       <- test backreference";
	u32* l_testBytes = new u32[strlen(lz77_enc_test)];
	for (i32 i = 0; i < strlen(lz77_enc_test); i++)
		l_testBytes[i] = (u32)lz77_enc_test[i];
	lz77_res* _lr = lz77_encode(l_testBytes, strlen(lz77_enc_test), 15, 256);
	std::cout << "LZ77 TEST RES" << std::endl;
	for (auto b : _lr->rStream)
		std::cout << (char)b << " ";
	std::cout << std::endl;


	for (int i = 0; i < 100; i++) {
		ZResult test = Zlib::Deflate(uTestStr, len, 15, 2);

		std::cout << "Compression Done!" << std::endl;

		for (i32 i = 0; i < test.len; i++) {
			std::cout << std::hex << test.bytes[i] << " ";
		}

		std::cout << std::dec << "\nInflate: " << std::endl;

		ZResult* testInflate = Zlib::Inflate((u32*)test.bytes, test.len);

		for (i32 i = 0; i < testInflate->len; i++) {
			std::cout << (char)testInflate->bytes[i] << " ";
		}

		delete testInflate;
	}

	std::cout << std::endl;
}
#else
void Huffman::DebugMain() {}
#endif

//huffman functions
HuffmanResult Huffman::Encode(char* bytes, size_t len, bool encodeTree) {
	HuffmanResult res;

	if (bytes == nullptr || len <= 0)
		return res;

	const size_t alphabetSz = 0x100;

	//create le tree
	u32* uBytes = new u32[len + 1];
	for (size_t i = 0; i < len; i++)
		(*(uBytes + i)) = (u32)bytes[i];
	uBytes[len] = 256;

	u32* charCounts = GetCharCount(uBytes, len, alphabetSz);

	HuffmanTreeNode* baseTree = GenerateBaseTree(charCounts, alphabetSz);
	HuffmanTreeNode* tree = CovnertTreeToCanonical(baseTree, alphabetSz);
	FreeTree(baseTree);

	BitStream stream = BitStream(0xff);

	if (encodeTree)
		writeTrees(stream, tree, nullptr);

	//encode data
	u32* bEnd = uBytes + len;

	u32* bitLens = getTreeBitLens(tree, alphabetSz);

	do {
		WriteVBitsToStream(
			stream,
			bitReverse(EncodeSymbol(
				*uBytes,
				tree
			), bitLens[*uBytes]),
			bitLens[*uBytes]
		);
	} while (++uBytes <= bEnd);

	stream.clip();

	//generate result
	res.len = stream.sz;
	res.tree = tree;
	res.bytes = new byte[res.len];

	for (size_t i = 0; i < stream.sz; i++)
		res.bytes[i] = (byte)stream.bytes[i];

	res.resultType = HUFFMAN_RESULT_ENCODE;

	return res;
};

HuffmanResult Huffman::Decode(char* bytes, size_t sz, bool useTree, HuffmanTreeNode* tree) {
	HuffmanResult res;
	HuffmanTreeNode* decodeTree;
	BitStream stream = BitStream(sz);
	HuffmanTreeNode** trees = nullptr;

	//get the decode tree
	for (i32 i = 0; i < sz; i++)
		stream.bytes[i] = bytes[i];

	if (!useTree) {
		trees = decodeTrees(stream);
		decodeTree = trees[0];
		FreeTree(trees[1]);
	}
	else
		decodeTree = tree;

	if (decodeTree == nullptr)
		return res;

	//now decode data
	std::vector<byte> rBytes;

	for (;;) {
		u32 sym = DecodeSymbol(stream, tree);
		if (sym == 256) break;
		rBytes.push_back((byte)sym);
	}

	res.len = rBytes.size();

	if (res.len > 0) {
		res.bytes = new byte[res.len];
		memcpy(res.bytes, rBytes.data(), res.len);
	}

	res.resultType = HUFFMAN_RESULT_DECODE;
	res.tree = decodeTree;

	//memory management
	FreeTree(decodeTree);
	if (trees != nullptr)
		delete[] trees;

	return res;
}

HuffmanResult::~HuffmanResult() {
	if (this->tree != nullptr)
		FreeTree(this->tree);

	if (this->bytes != nullptr)
		delete[] this->bytes;
}