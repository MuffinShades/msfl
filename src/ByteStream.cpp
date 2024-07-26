#include "ByteStream.hpp"

void ByteStream::allocNewChunk()
{
    this->allocBytes(this->chunkSz);
}

void ByteStream::allocBytes(size_t sz)
{
    if (this->bytes == nullptr || this->len <= 0) {
        this->allocSz = 1;
        if (this->bytes != nullptr)
            delete[] this->bytes;
        
        this->bytes = new byte[1];
        ZeroMem(this->bytes, 1);
    }
    if (sz <= 0)
        return;
    this->allocSz += sz;
    byte *tmp = new byte[this->allocSz];
    ZeroMem(tmp, this->allocSz);
    if (this->len > 0) 
        memcpy(tmp, this->bytes, this->len * sizeof(byte));
    if (this->bytes != nullptr)
        delete[] this->bytes;
    this->bytes = tmp;
}

ByteStream::ByteStream(byte *dat, size_t sz)
{
    if (dat == nullptr || sz <= 0)
        return;

    this->bytes = new byte[sz];
    this->len = sz;
    this->allocSz = this->len;
    memcpy(this->bytes, dat, this->len);
}

ByteStream::ByteStream()
{
    this->bytes = nullptr;
    this->len = 0;
    this->allocSz = 0;
};

byte ByteStream::readByte()
{
    if (this->readPos >= this->len - 1)
        return NULL;

    return this->bytes[this->readPos++];
}

unsigned long long ByteStream::readBytesAsVal(size_t nBytes)
{
    if (nBytes <= 0)
        return NULL;
    unsigned long long res = 0u;
    switch (this->mode)
    {
    case bmode_BigEndian:
    {
        for (int i = nBytes - 1; i >= 0; i--)
            res |= (this->readByte() << (i * 8));
        break;
    }
    case bmode_LittleEndian:
    {
        for (int i = 0; i < nBytes; i++)
            res |= (this->readByte() << (i * 8));
        break;
    }
    }
    return res;
}

short ByteStream::readInt16()
{
    return (short)this->readBytesAsVal(sizeof(short));
}

unsigned short ByteStream::readUInt16()
{
    return (unsigned)this->readInt16();
}

int ByteStream::readInt32()
{
    return (int)this->readBytesAsVal(sizeof(int));
}

unsigned int ByteStream::readUInt32()
{
    return (unsigned)this->readInt32();
}

int64_t ByteStream::readInt64()
{
    return (int64_t)this->readBytesAsVal(sizeof(int64_t));
}

uint64_t ByteStream::readUInt64()
{
    return (unsigned)this->readInt64();
}

void ByteStream::writeByte(byte b)
{
    if (++this->len > this->allocSz)
        this->allocNewChunk();
    this->bytes[this->len - 1] = b;
}

void ByteStream::writeNBytesAsVal(unsigned long long v, size_t nBytes)
{
    if (nBytes <= 0)
        return;
    switch (this->mode)
    {
    case bmode_BigEndian:
    {
        for (int i = nBytes - 1; i >= 0; i--)
            this->writeByte((v >> (i*8)) & 0xff);
        break;
    }
    case bmode_LittleEndian:
    {
        for (int i = 0; i < nBytes; i++)
            this->writeByte((v >> (i*8)) & 0xff);
        break;
    }
    }
}

void ByteStream::writeInt16(short v)
{
    this->writeNBytesAsVal(v, sizeof(short));
}

void ByteStream::writeUInt16(unsigned short v)
{
    this->writeNBytesAsVal(v, sizeof(unsigned short));
}

void ByteStream::writeInt32(int v)
{
    this->writeNBytesAsVal(v, sizeof(int));
}

void ByteStream::writeUInt32(unsigned int v)
{
    this->writeNBytesAsVal(v, sizeof(unsigned int));
}

void ByteStream::writeInt64(int64_t v)
{
    this->writeNBytesAsVal(v, sizeof(long));
}

void ByteStream::writeUInt64(uint64_t v)
{
    this->writeNBytesAsVal(v, sizeof(unsigned long));
}

void ByteStream::clip()
{
    if (this->allocSz == this->len)
        return;
    byte *tmp = new byte[this->len];
    memcpy(tmp, this->bytes, this->len);
    this->allocSz = this->len;
    delete[] this->bytes;
    this->bytes = tmp;
}

void ByteStream::writeBytes(byte *dat, size_t sz)
{
    if (dat == nullptr || sz <= 0)
        return;
    const size_t padding = 0xff; // padding yk
    const size_t pos = this->len;
    if ((this->len + sz) > this->allocSz)
        this->allocBytes((this->len + sz + padding) - this->allocSz);

    // memcpy
    memcpy(this->bytes + pos, dat, sz);
    this->len = pos+sz;
    std::cout << this->len << std::endl;
}

ByteStream::~ByteStream()
{
    
}

size_t ByteStream::seek(size_t pos) {
    size_t old = this->readPos;
    this->readPos = pos;
    return old;
}

size_t ByteStream::skipBytes(size_t nBytes) {
		const size_t oPos = this->readPos;
		this->readPos += nBytes;
		this->readPos = min(this->readPos, this->len - 1);
		return oPos;
	}

//welp