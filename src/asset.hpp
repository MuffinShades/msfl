/**
 * ---------------------------------------
 * Game Asset file format
 * 
 * Written by muffinshades for msfl July, 2024
 * 
 * Copyright (c) muffinshades, All Rights Reserved.
 * 
 * This code is free to use as long as credit is given
 * to the author, muffinshades.
 * 
 * Version 1.0.0
 * ---------------------------------------
 */

#include "balloon.hpp"
#include "jparse.hpp"
#include "ByteStream.hpp"
#include "FileWriter.h"
#include "Date.hpp"

struct AssetDescriptor {
    size_t sz = 0;
    std::string fileType = "";
    Date created;
    Date modified;
    std::string fname = "";
    int compressionType = 2;
    std::string aId = "";
};

struct Asset {
    AssetDescriptor inf;
    size_t sz;
    byte *bytes;
};

enum _aContainerType {
    _aTypeAsset,
    _aTypeContainer
};

class AssetContainer {
private:
    enum _aContainerType _ty = _aTypeContainer;
    Asset _assetData;
    std::string id = "";
public:
    std::vector<AssetContainer*> assets;
    AssetContainer *GetNode(std::string id);
    Asset *GetAsset(std::string id);
    Asset *GetAssetData() {
        return &this->_assetData;
    };
    void SetAssetData(Asset a);
    void AddAsset(std::string id, byte *data, size_t sz);
    void AddAsset(std::string id, std::string fSrc);
    void AddAsset(std::string id, AssetDescriptor desc);
    AssetContainer *AddContainer(std::string id);
    void SetId(std::string id);
    std::string GetId();
    AssetContainer(std::string id = "");
    ~AssetContainer();
    size_t getNAssets() {
        return this->assets.size();
    }
    enum _aContainerType getType() {
        return this->_ty;
    }
};

class AssetStruct {
private:
    AssetContainer *map;
public:
    Asset GetAsset(std::string path);
    void AddAsset(std::string path, byte *data, size_t sz);
    void AddAsset(std::string path, std::string fSrc);
    void AddAsset(std::string path, AssetDescriptor desc);
    AssetContainer *GetRoot() {
        return this->map;
    };
    AssetStruct(){
        this->map = new AssetContainer("root");
    };
    ~AssetStruct(){
        if (this->map != nullptr) {
           delete this->map;
           this->map = nullptr;
        }
    };
};

class AssetFile {
private:
    byte *data = nullptr;
    size_t sz = 0;
    size_t rootOffset = 0;
public:
    AssetStruct constructStruct();
    Asset getAsset(std::string path);
    AssetContainer constructContainer(std::string path);
    byte *getDataPtr();
};

class AssetParse {
public:
    static int WriteToFile(std::string src, AssetStruct *dat); //done
    static byte *WriteToBytes(AssetStruct *dat);
    static int WriteToFile(std::string src, std::string jsonMap); //done
    static AssetStruct ParseAssetFile(std::string src); //done
    static AssetStruct ParseDat(byte *dat, size_t sz);
    static AssetFile ReadFile(std::string src);
    static JStruct ReadFileMapAsJson(std::string src);
    static JStruct ReadFileMapAsJson(byte *dat, size_t sz);
    static Asset ExtractAssetFromFile(std::string src, std::string path);
    static Asset ExtractAssetFromData(byte *dat, size_t sz);
};