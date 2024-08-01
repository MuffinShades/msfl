#include "asset.hpp"
/**
 * 
 * Asset File Format v 1.0.0
 * 
 * Written by muffinshades, Copyright muffinshades 2024 
 * All Rights Reserved
 * 
 * This is the encoder, decoder, and utility functions for
 * the Asset / AssetPack format. 
 * 
 */

const std::string _asset_errs[] = {
    "Invalid path length",
    "Asset does not exist",
    "Asset is a container"
};

//severity levels
// 0 - not severe
// 1 - actuall error
const int _asset_err_severity[] = {
    1,
    0,
    1
};

struct fPos {
    size_t pos = 0;
    int error_code = 0;
};

fPos _createFPosErr(int errCode) {
    return {
        .error_code = errCode,
        .pos = 0
    };
}

struct Version {
public:
    int mainVersion, subVersion, protoVersion;
    Version(int mv, int sv, int pv) : mainVersion(mv), subVersion(sv), protoVersion(pv) {}
    unsigned long long getLong() {
        return (this->mainVersion << 32) | (this->subVersion << 16) | (this->protoVersion);
    }
};

class _AssetHeader {
public:
    const unsigned int sig = 0x56d102fa; //random number i came up with
    const char fStr[4] = {'M','S','A','P'}; //MSAP -> muffin shades asset package (denotes it's a .pak, .mpak, .msap, or .ap)
    size_t nAssets = 0; //denotes how many assets are going to be in the next container
    size_t rootOffset = 0; //offset of the root container
    Version fVersion = Version(1, 0, 1); //file version
    size_t locOffset = 0;
    _AssetHeader(){}
};

AssetContainer *AssetContainer::AddContainer(std::string id) {
    if (this->_ty != _aTypeContainer) //verify it's container and not something else
        return nullptr;

    AssetContainer *n = new AssetContainer(id);
    this->assets.push_back(n);

    return n;
}

//used in parsing paths
std::vector<std::string> _splitString(std::string str, const char delim) {
    std::vector<std::string> res;
    const size_t len = str.length();
    std::string collector = "";
    const char *cStr = str.c_str();

    for (size_t i = 0; i < len; i++) {
        if (cStr[i] == delim) {
            res.push_back(collector);
            collector = "";
        } else
            collector += cStr[i];
    }

    res.push_back(collector);

    return res;
}

std::string AssetContainer::GetId() {
    return this->id;
}

void AssetContainer::SetId(std::string id) {
    if (id.length() <= 0) return;

    this->id = id;
}

AssetContainer::AssetContainer(std::string id) {
    this->SetId(id);
}

AssetContainer::~AssetContainer() {
    //memory management
    for (AssetContainer *t : this->assets) {
        if (t != nullptr)
            delete t;
    }
}

AssetContainer *AssetContainer::GetNode(std::string id) {
    for (auto *n : this->assets) {
        if (n == nullptr || !n) continue;

        if (n->GetId() == id)
            return n;
    }

    return nullptr;
}

void AssetContainer::SetAssetData(Asset a) {
    this->_ty = _aTypeAsset;
    this->_assetData = a;

    //remove old child nodes since this is an asset node
    for (AssetContainer *asset : this->assets) {
        if (asset != nullptr)
            delete asset;
    }
}

Asset construct_asset(byte *data, size_t sz, AssetDescriptor desc = {}) {
    assert(sz > 0);
    Asset res;
    res.bytes = new byte[sz];
    memcpy(res.bytes, data, sz);
    res.sz = sz;
    res.inf = desc;
    return res;
}

//add asset to a file
void AssetStruct::AddAsset(std::string path, byte *data, size_t sz, AssetDescriptor desc) {
    if (data == nullptr || sz <= 0 || path.length() <= 0) //data check
        return;

    std::vector<std::string> route = _splitString(path, '.');

    //now traverse path and create containers when needed
    AssetContainer *current = this->map;

    if (!this->map)
        this->map = new AssetContainer("root");
    
    for (auto& nId : route) {
        if (nId.length() <= 0) return; //INVALID LENGTH
        AssetContainer *nxt = nullptr;
        //first check for container
        if ((nxt = current->GetNode(nId)) != nullptr)
            current = nxt;
        else
            current = current->AddContainer(nId);
    }

    if (current == nullptr)
        return;

    desc.aId = current->GetId();

    current->SetAssetData(
        construct_asset(data, sz, desc)
    );
}

//add asset based on AssetDescriptor
void AssetStruct::AddAsset(std::string path, AssetDescriptor desc) {
    if (path.length() <= 0) //data check
        return;

    std::vector<std::string> route = _splitString(path, '.');

    //now traverse path and create containers when needed
    AssetContainer *current = this->map;

    if (!this->map)
        this->map = new AssetContainer("root");
    
    for (auto& nId : route) {
        if (nId.length() <= 0) return; //INVALID LENGTH
        AssetContainer *nxt = nullptr;
        //first check for container
        if ((nxt = current->GetNode(nId)) != nullptr)
            current = nxt;
        else
            current = current->AddContainer(nId);
    }

    if (current == nullptr)
        return;

    current->SetAssetData(
        {
            .inf = desc,
            .bytes = nullptr,
            .sz = 0
        }
    );
}

//add file asset struct thing
void AssetStruct::AddAsset(std::string path, std::string src) {
    if (src.length() <= 0 || path.length() <= 0)
        return;

    //get file contents
    arr_container<byte> fDat = FileWriter::ReadBytesFromBinFile(src);
    FileInfo fInfo = FilePath_int::getFileInfo(src);

    AssetDescriptor fDesc = {
        .created = fInfo.creationDate,
        .modified = fInfo.modifiedDate,
        .fname = fInfo.name,
        .fileType = fInfo.type
    };

    std::cout << "FDAT SZ: " << fDat.sz << std::endl;

    if (fDat.dat == nullptr || fDat.sz <= 0) {
        if (fDat.dat != nullptr)
            delete[] fDat.dat;

        return;
    }

    //add le asset
    this->AddAsset(path, fDat.dat, fDat.sz, fDesc);
    delete[] fDat.dat;
}

//function to add a asset
void AssetContainer::AddAsset(std::string id, byte *data, size_t sz, AssetDescriptor desc) {
    for (auto* n : this->assets) {
        if (n->id == id)
            return;
    }

    desc.aId = id;

    AssetContainer *item = this->AddContainer(id);
    item->SetAssetData(
        construct_asset(data, sz, desc)
    );
}

//adds asset based on asset descriptor
void AssetContainer::AddAsset(std::string id, AssetDescriptor desc) {
    for (auto* n : this->assets) {
        if (n->id == id)
            return;
    }

    AssetContainer *item = this->AddContainer(id);
    item->SetAssetData(
        {
            .inf = desc,
            .bytes = nullptr,
            .sz = 0
        }
    );
}

enum _ChunkType {
    _cty_Container,
    _cty_Asset,
    _cty_max
};

#include "ptrconverter.hpp"

void write_header(_AssetHeader header, ByteStream *stream) {
    if (stream == nullptr)
        return;

    //write file label (MSAP)
    byte *fBytes = ptrconverter::convertTo<const char, byte>(
        header.fStr, 4
    );

    stream->writeBytes(
        fBytes, 4
    );

    //write sig
    stream->writeUInt32((unsigned int)header.sig);
    //write file version
    stream->writeUInt64(header.fVersion.getLong());
    
    //write 64 blank bytes that may be used later idk
}

const std::string chunkTypeStrs[2] = {
    "CONT",
    "ASET"
};

//writes chunks yk
void write_chunk(enum _ChunkType chunkType, ByteStream *chunkStream, ByteStream *outStream) {
    if (chunkType >= _cty_max || chunkType < 0)
        return;

    //write chunk label / identifier
    const std::string chunkLabel = chunkTypeStrs[chunkType];

    outStream->writeByte(chunkLabel.length() & 0xff);
    outStream->writeBytes(
        reinterpret_cast<byte*>(
            const_cast<char*>(chunkLabel.c_str())
        ),
        chunkLabel.length()
    );

    //write chunk size
    const size_t cSz = chunkStream->getSize();
    outStream->writeUInt32(cSz);

    //TODO: crc32
    outStream->writeUInt32(0xffffffff);

    //write chunk data
    outStream->writeBytes(chunkStream->getBytePtr(), cSz);
    chunkStream->free();
}

const int construct_FINFO(bool encryption, bool streamable, int compressionType) {
    return 
        ((compressionType & 3) << 2) | 
        (((int) streamable & 1) << 1) | 
        ((int) encryption & 1);
}

void write_f_string(ByteStream *stream, std::string str) {
    size_t fTypeSz = str.length();

    if (fTypeSz > 0xff)
        fTypeSz = 0;

    stream->writeByte(fTypeSz);

    if (fTypeSz > 0)
        stream->writeBytes(
            reinterpret_cast<byte*>(
                const_cast<char*>(
                    str.c_str()
                )
            ),

            fTypeSz
        );
}

//TODO: create this function to write assets
fPos write_asset(AssetContainer *container, ByteStream *stream) {
    ByteStream aStream;
    Asset *aData = container->GetAssetData();

    //write chunk id
    const std::string aId = container->GetId();
    const size_t aIdSz = aId.length();

    if (aIdSz > 0xff)
        return _createFPosErr(1);

    aStream.writeByte(aIdSz);
    aStream.writeBytes(
        reinterpret_cast<byte*>(
            const_cast<char*>(aId.c_str())
        ), aIdSz
    );

    //compress data
    byte *compressedBytes = nullptr;
    size_t compressedSz = 0;

    switch (aData->inf.compressionType) {
        case 0: { //no compression
            compressedBytes = new byte[aData->sz];
            memcpy(compressedBytes, aData->bytes, aData->sz);
            compressedSz = aData->sz;
            break;
        }
        default: { //zlib
            u32* uBytes = ptrconverter::convertTo<byte, u32>(aData->bytes, aData->sz);
            ZResult zrs = Zlib::Deflate(
                uBytes,
                aData->sz,
                15,
                aData->inf.compressionType
            );
            delete[] uBytes;
            compressedBytes = ptrconverter::convertTo<u32, byte>(zrs.bytes, zrs.len);
            compressedSz = zrs.len;
            break;
        }
    }

    if (compressedBytes == nullptr || compressedSz <= 0)
        return _createFPosErr(2);

    //write compressed size
    aStream.writeUInt32(compressedSz);
    aStream.writeUInt32(aData->sz);

    //write file descriptor
    aStream.writeUInt64(aData->inf.created.getLong());
    aStream.writeUInt64(aData->inf.modified.getLong());

    //write file type
    write_f_string(&aStream, aData->inf.fileType);

    //write fname
    write_f_string(&aStream, aData->inf.fname);

    //finfo
    const byte F_INFO = construct_FINFO(false, false, aData->inf.compressionType); //TODO: add encryption and streamability

    aStream.writeByte(F_INFO);

    //write data
    aStream.writeBytes(compressedBytes, compressedSz);

    //write chunk and clean up
    delete[] compressedBytes;
    fPos rPos = {
        .pos = stream->getSize()
    };
    write_chunk(_cty_Asset, &aStream, stream); //TODO: make this not free streams or what not
    
    return rPos;
}

/**
 *
 * Writes container location
 * 
 * 1 byte -> id size
 * n bytes -> id
 * 1 byte -> size of location
 * n bytes -> location
 * 
 */
void write_loc(std::string id, size_t loc, ByteStream *stream) {
    if (stream == nullptr)
        return;

    const size_t nLocBytes = GetNumSz(loc), idSz = id.length();

    //write id
    stream->writeByte(idSz & 0xff);
    stream->writeBytes(
        reinterpret_cast<byte*>(
            const_cast<char*>(id.c_str())
        ), idSz
    );

    //write location
    stream->writeByte(nLocBytes);
    stream->writeNBytesAsVal(loc, nLocBytes);
}

/**
 * 
 * Writes a asset container
 * 
 * retunrs le position it was written
 * at for storing in the file for fast
 * read speeds
 * 
 */
fPos write_container(AssetContainer *container, ByteStream *stream) {
    ByteStream cStream; //stream for container
    const size_t nAssets = container->getNAssets();

    //-------Write Container Header------------
    cStream.writeUInt16(nAssets); //write number of assets
    const std::string cId = container->GetId();
    const size_t idLen = cId.length();

    if (idLen > 0xff) //TODO: error stuff
        return _createFPosErr(1); //ERROR: invalid length
    
    //container id
    cStream.writeByte(idLen);
    cStream.writeBytes(
        reinterpret_cast<byte*>(
            const_cast<char*>(cId.c_str())
        ), idLen
    );

    //write the child items
    for (auto *a : container->assets) {
        if (a == nullptr)
            continue;

        switch (a->getType()) {
            case _aTypeAsset: {
                //write asset chunk
                fPos aPos = write_asset(a, stream);

                if (aPos.error_code != 0) //pass error code if one is returned
                    return _createFPosErr(aPos.error_code);

                write_loc(
                    a->GetId(), 
                    aPos.pos,
                    &cStream
                );
                break;
            }
            case _aTypeContainer: {
                //write container chunk
                fPos cPos = write_container(a, stream);

                if (cPos.error_code != 0) //pass error code if one is returned
                    return _createFPosErr(cPos.error_code);

                write_loc(  //write le location
                    a->GetId(),
                    cPos.pos,
                    &cStream
                );
                break;
            }
            default:
                return _createFPosErr(2); //ERROR 2: invalid container type
        }
    }

    //res
    fPos res = {
        .pos = stream->getSize(),
        .error_code = 0
    };

    //write chunk
    write_chunk(_cty_Container, &cStream, stream);
    cStream.free();

    //return yk
    return res;
}

int AssetParse::WriteToFile(std::string src, AssetStruct *dat) {
    if (dat == nullptr)
        return 1; //error code  1 >_>

    ByteStream oStream, datStream;

    _AssetHeader head;

    //create and write header
    write_header(head, &oStream);

    //now write the root to the data stream
    fPos rootPos = write_container(dat->GetRoot(), &datStream);
    oStream.writeUInt32(rootPos.pos + oStream.getSize() + sizeof(u32)); //write position of root for streaming capability

    //write le data stream
    oStream.writeBytes(datStream.getBytePtr(), datStream.getSize());
    datStream.free();

    //TODO: write the bytes to the output file
    FileWriter::WriteToBinFile(src, oStream.getBytePtr(), oStream.getSize());
    oStream.free();

    return 0;
}

Asset _make_err_asset(int err_code) {
    Asset res;

    //TODO: create errors on assets

    return res;
}

Asset _make_null_asset() {
    return {};
}

Asset AssetStruct::GetAsset(std::string path) {
    if (path.length() <= 0)
        return _make_err_asset(1);

    //get path
    std::vector<std::string> route = _splitString(path, '.');
    size_t step = 0;

    AssetContainer *current = this->map;

    for (;;) {
        //get next
        AssetContainer *nxt = current->GetNode(route[step++]);

        if (nxt == nullptr)
            return _make_err_asset(2);
        else
            current = nxt;

        if (step >= route.size())
            break;
    }

    //return le found asset
    if (current->getType() != _aTypeAsset)
        return _make_err_asset(3);

    return *current->GetAssetData();
}

Asset *AssetContainer::GetAsset(std::string id) {
    if (id.length() <= 0)
        return nullptr;

    AssetContainer *target = this->GetNode(id);

    if (target->getType() != _aTypeAsset)
        return nullptr;
    return target->GetAssetData();
}

#include "jparse.hpp"

struct _jAsset {
    std::string path, src;
};

//json struct interator to get the file map from the json struct
void _jfItr(std::vector<_jAsset>* _toAdd, JStruct currentStruct, std::string cPath) {
    for (auto& tok : currentStruct.body) {
        if (tok.body != nullptr)
            _jfItr(_toAdd, *tok.body, cPath + (tok.label + "."));
        else
            _toAdd->push_back({
                .path = cPath + (tok.label + "."),
                .src = tok.rawValue
            });
    }
}

/**
 * 
 * read_descriptor
 * 
 * reads asset descriptor from stream
 * AssetDescriptor:
 *  
 * 
 */

//File:
//
// asset id len
// asset id
// data size
// created
// modified
// ftypelen
// ftype
// fInfo
// data
AssetDescriptor read_descriptor(ByteStream *stream) {
    if (stream == nullptr)
        return {};

    const size_t idLen = stream->readByte();
    if (idLen <= 0) return {}; //nothing there ;-;

    std::string aId = stream->readStr(idLen);

    //read in date things
    u64 creLong = stream->readUInt64(), modLong = stream->readUInt64();

    //file type
    const size_t ftl = stream->readByte();
    std::string fType = "";
    if (ftl > 0)
        fType = stream->readStr(ftl);

    //file name
    const size_t fnl = stream->readByte();
    std::string fName = "";
    if (fnl > 0)
        fName = stream->readStr(fnl);

    //construct result
    return {
        .created = Date(creLong),
        .modified = Date(modLong),
        .fileType = fType,
        .fname = fName,
        .aId = aId
    };
}

/**
 * 
 * AssetParse::WriteToFile (jsonMap)
 * 
 * writes a asset file based on a .json file
 * instead of a AssetStruct. Good for first
 * time creation of asset files
 * 
 */
i32 AssetParse::WriteToFile(std::string src, std::string jsonMap) {
    if (src.length() <= 0)
        return 1;

    JStruct jMap = jparse::parseStr(jsonMap.c_str());
    AssetStruct fStruct; //file struct

    std::vector<_jAsset> _toAdd;

    _jfItr(&_toAdd, jMap, ""); //get all the paths from the json file


    //add elements and their path
    for (_jAsset& aTarget : _toAdd) {
        std::string path = aTarget.path.substr(0, aTarget.path.length() - 1);
        std::cout << "[719] ASSET PATH: " << path << " "  << aTarget.src << std::endl;
        fStruct.AddAsset(path, aTarget.src);
    }

    return AssetParse::WriteToFile(src, &fStruct);
}

//parsing le asset file... fun
AssetStruct AssetParse::ParseAssetFile(std::string src) {
    //TODO: this function
}

bool sig_verify(std::string _fStr, _AssetHeader _h) {
    size_t _sigPos = 0;
    for (char c : _fStr)
        if (c != _h.fStr[_sigPos++])
            return false;
    return true;
}

//read header from file
_AssetHeader read_header(ByteStream *stream) {
    size_t jPos = stream->seek(0);

    _AssetHeader _h = {};

    std::string _fStr = stream->readStr(4);

    //verify a valid signiture
    if (!sig_verify(_fStr, _h))
        return _h;

    //now read rest of file header stuff
    u32 _fSig = stream->readUInt32();

    if (_fSig != _h.sig)
        return _h;

    //read file version
    u64 fVersion = stream->readUInt64();

    const u32 _vMask = GMask(sizeof(i16) * 8);

    _h.fVersion = Version(
        (fVersion >> 32) & _vMask,
        (fVersion >> 16) & _vMask,
        (fVersion >>  0) & _vMask
    );

    //get important offsets for stuff
    _h.rootOffset = stream->readUInt32();
    _h.locOffset = stream->tell();

    std::cout << "[772] Asset File V: " << _h.fVersion.mainVersion << "." << _h.fVersion.subVersion << "." << _h.fVersion.protoVersion << std::endl;

    return _h;
}

//Asset:
//
// container label len
// container label
// nassets
// children / assets...
// 1 byte - len of name
// name
// 1 byte - size of offset
// offset

struct _fChunk {
    _ChunkType ty;
    size_t datSz;
    u32 crc32;
};

iErrorWrap<_fChunk> read_chunk(ByteStream *stream) {
    assert(stream != nullptr);
    _fChunk res = {
        .ty = _cty_max
    };

    const size_t ll = stream->readByte();

    if (ll <= 0)
        return iErrorWrap<_fChunk>(res, iErrorWrap<i32>::CreateGenericError(1, ""));

    //get chunk type
    std::string chunkLbl = stream->readStr(ll);
    std::cout << "label len: " << ll << std::endl;

    std::cout << "[808] Found Chunk: " << chunkLbl << std::endl;

    for (size_t i = 0; i < _cty_max; i++) 
        if (chunkLbl == chunkTypeStrs[i]) {
            res.ty = (_ChunkType) i;
            break;
        }

    if (res.ty == _cty_max)
        return iErrorWrap<_fChunk>(res, iErrorWrap<i32>::CreateGenericError(1, ""));

    //get chunk size
    res.datSz = stream->readUInt32();
    res.crc32 = stream->readUInt32();

    return iErrorWrap<_fChunk>(res, nullptr);
}

void map_file(ByteStream *stream, _AssetHeader _h, AssetStruct *s, std::string path = "") {
    _fChunk tChunk = read_chunk(stream);
    switch (tChunk.ty) {
        //asset mapping
        case _cty_Asset: {
            AssetDescriptor desc = read_descriptor(stream);

#ifdef MSFL_ASSET_DEBUG
            std::cout << "[834] Asset Description: " 
            << "\n\t" << "aId: " << desc.aId 
            << "\n\t" << "compressionType: " << desc.compressionType
            << "\n\t" << "Creation: " << desc.created.getString()
            << "\n\t" << "Modified: " << desc.modified.getString()
            << "\n\t" << "File Type: " << desc.fileType
            << "\n\t" << "File Name: " << desc.fname
            << std::endl;
#endif

            s->AddAsset(path + desc.aId, desc);
            break;
        }
        //asset container mapping
        case _cty_Container: {
            size_t nc = stream->readUInt16();
            const size_t n_len = stream->readByte();
            if (n_len <= 0) return;
            const std::string n = stream->readStr(n_len);
            path += (n + ".");
            if (nc > 0)
                while (nc--) {
                    size_t an_len = stream->readByte(), o_len; // -> 617
                    if (an_len <= 0) continue;
                    std::string iId = stream->readStr(an_len); // -> 618
                    o_len = stream->readByte(); // -> 619
                    const size_t off = (unsigned) stream->readBytesAsVal(o_len) + _h.locOffset,
                                 r_pos = stream->seek(off);
                    map_file(stream, _h, s, path);
                    stream->seek(r_pos);
                }
            else
                s->AddAsset(path.substr(0, path.length() - 1), ""); //add null asset or branch with no fruit
            break;
        }
        default:
            return; //invalid chunk
    }
}

JStruct *adesc_to_jstruct(AssetDescriptor desc) {
    JStruct *res = new JStruct();

    res->body.push_back(JToken("fileType", desc.fileType));
    res->body.push_back(JToken("assetId", desc.aId));
    res->body.push_back(JToken("compressionType", std::to_string(desc.compressionType), JType_Int));
    res->body.push_back(JToken("created", desc.created.getString()));
    res->body.push_back(JToken("modified", desc.modified.getString()));
    res->body.push_back(JToken("fileName", desc.fname));

    return res;
}

void astruct_to_jstruct(AssetContainer *node, JStruct *o) {
    if (node == nullptr || o == nullptr) return;

    for (auto* c : node->assets) {
        std::cout << "[896] BANANA" << std::endl;
        if (c->getType() != _aTypeAsset) {
            JToken nTok = JToken();
            nTok.body = new JStruct();
            nTok.label = c->GetId();
            o->body.push_back(nTok);
            astruct_to_jstruct(c, nTok.body); //go to next node
        } else {
            JToken nTok = JToken();
            nTok.label = c->GetId();
            
            //convert
            nTok.body = adesc_to_jstruct(c->GetAssetData()->inf);

            o->body.push_back(nTok);
        }
    }
}

JStruct AssetParse::ReadFileMapAsJson(byte *dat, size_t sz) {
    ByteStream stream = ByteStream(dat, sz);

    //read file header
    _AssetHeader fHeader = read_header(&stream);

    //get position of root node
    const size_t rootPos = fHeader.rootOffset;
    stream.seek(rootPos);

    JStruct res;

    AssetStruct intStruct;
    map_file(&stream, fHeader, &intStruct);

    //convert asset struct to JStruct
    AssetContainer *root = intStruct.GetRoot();
    if (root == nullptr) return res;
    astruct_to_jstruct(root, &res);

    return res;
}

JStruct AssetParse::ReadFileMapAsJson(std::string src) {
    if (src.length() <= 0)
        return {};

    //get file contents
    arr_container<byte> fDat = FileWriter::ReadBytesFromBinFile(src);

    if (fDat.dat == nullptr || fDat.sz <= 0) {
        if (fDat.dat != nullptr)
            delete[] fDat.dat;

        return {};
    }

    JStruct s = AssetParse::ReadFileMapAsJson(fDat.dat, fDat.sz);
    delete[] fDat.dat;

    return s;
}