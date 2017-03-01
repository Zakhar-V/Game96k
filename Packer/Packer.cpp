/*
Copyright (С) 2016, Volkov Zakhar

Packer - Многофункциональная утилита по упаковке файлов.
1. Создает один файл из нескольких
2. Сжимает полученный файл
3. Создает заголовочный файл из полученного файла
*/

#include "lz4.h"
#include "lz4hc.h"
#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <string>

#define USE_HASH // save hash instead name
//#define USE_32BIT_CHECKSUM // use 32 bit checksum

#ifdef USE_32BIT_CHECKSUM
typedef uint32_t hash_t;
#	define HASH_FMT "0x%08x"
#else
typedef uint16_t hash_t;
#	define HASH_FMT "0x%04x"
#endif	


const int INFO_SIZE = sizeof(hash_t) + sizeof(uint16_t);

char CharLower(char _ch)
{
	return (_ch >= 'A' && _ch <= 'Z') || (_ch >= 'a' && _ch <= 'z') || ((uint8_t)_ch >= 0xc0) ? (_ch | 0x20) : _ch;
}

uint32_t StringHash(const char* _str, uint32_t _hash = 0)
{
	if (_str)
	{
		while (*_str)
#ifdef USE_32BIT_CHECKSUM // SDBM hash 32 bit
			_hash = CharLower(*_str++) + (_hash << 6) + (_hash << 16) - _hash;
#else // BSD checksum 16 bit
			_hash = ((_hash >> 1) + ((_hash & 1) << 15) + CharLower(*_str++)) & 0xffff;
#endif
	}
	return _hash;
}

bool SaveFile(const char* _name,  const char* _type, const void* _data, size_t _size, bool _saveSize, size_t _rawSize = 0)
{
	bool _result = false;
	FILE* _file = fopen(_name, "wb");
	if (_file)
	{
		uint16_t _size16 = (uint16_t)_rawSize;
		if ((!_saveSize || fwrite(&_size16, 1, 2, _file) == 2) && (!_size || fwrite(_data, 1, _size, _file) == _size))
		{
			_result = true;
			printf("Save %s file: \"%s\"\n", _type, _name);
		}
		else
			printf("Couldn't write %s file \"%s\"\n", _type, _name);

		fclose(_file);
	}
	else
		printf("Couldn't create %s file \"%s\"\n", _type, _name);

	return _result;
}

int main(int _argc, const char** _argv)
{
	// skip exe filename
	if (_argc)
		_argc--, _argv++;

#if 0 // test
	const char* _args[] = { "test.out", "test.in" };
	_argv = _args;
	_argc = 2;
#endif

	// options
	bool _saveRawFile = true;
	bool _saveRawFileAlso = false;
	bool _saveInlFile = false;
	bool _useCompression = false;

	while (**_argv == '/' && _argc > 0)
	{
		const char* _opt = *_argv++;
		_argc--;
		if (!strcmp(_opt, "/r")) // save raw file
		{
			_saveRawFile = true;
		}
		else if (!strcmp(_opt, "/ra")) // save raw file also
		{
			_saveRawFileAlso = true;
		}
		else if (!strcmp(_opt, "/i")) // save inl file
		{
			_saveInlFile = true;
		}
		else if (!strcmp(_opt, "/c")) // compress pack
		{
			_useCompression = true;
		}
		else
		{
			printf("Unknown option '%s'\n", _opt);
			printf("Supported options: /r /ra /i /c\n");
			return 0;
		}
	}

	// no args
	if (_argc < 1)
	{
		printf("Usage: /options output file1, file2 ...\n");
		printf("/r - save raw file as output (default)\n");
		printf("/ra - save also raw file as output.raw (when /i)\n");
		printf("/i - save inl file as output (ignore /r)\n");
		printf("/c - use lz4 compression\n");
		return 0;
	}

	// output filename
	const char* _outputFilename = *_argv++;
	--_argc;

	// additional info
	int _numFiles = 0;
	int _serviceInfoSize = 0;

	// make package
	std::vector<char> _uncompressed;
	while (_argc--)
	{
		const char* _name = *_argv++;
		FILE* _file = fopen(_name, "rb");
		if (!_file)
		{
			printf("Couldn't open file \"%s\"\n", _name);
			return -1;
		}

		fseek(_file, 0, SEEK_END);
		long _size = ftell(_file);
		fseek(_file, 0, SEEK_SET);
		if (_size > 0xffff)
		{
			printf("Too big file \"%s\"\n", _name);
			return -1;
		}

		size_t _offset = _uncompressed.size();
#ifdef USE_HASH
		_uncompressed.resize(_offset + INFO_SIZE + _size);
		char* _dstp = (&_uncompressed[0]) + _offset;
		*(hash_t*)_dstp = StringHash(_name);
		_dstp += sizeof(hash_t);
#else
		size_t _len = strlen(_name);
		_uncompressed.resize(_offset + _len + 3 + _size);
		char* _dstp = (&_uncompressed[0]) + _offset;
		memcpy(_dstp, _name, _len + 1);
		_dstp += _len + 1;
#endif
		*(uint16_t*)_dstp = (uint16_t)_size;
		_dstp += 2;
		if (fread(_dstp, 1, _size, _file) != _size)
		{
			printf("Couldn't read file \"%s\"\n", _name);
			return -1;
		}
		fclose(_file);

		printf("Add file: \"%s\" (" HASH_FMT "), %d bytes\n", _name, StringHash(_name), _size);
		++_numFiles;
#ifdef USE_HASH
		_serviceInfoSize += INFO_SIZE;
#else
		_serviceInfoSize += _len + 3;
#endif
	}

	// compress package
	std::vector<char> _compressed;
	int _compressedSize = _uncompressed.size();
	if (_useCompression)
	{
		_compressed.resize(LZ4_compressBound(_uncompressed.size()));
		_compressedSize = _uncompressed.empty() ? 0 : LZ4_compressHC(&_uncompressed[0], &_compressed[0], _uncompressed.size());
	}
	else
	{
		_compressed = _uncompressed;
	}

	// log
	{
		printf("Uncompressed size: %d bytes (Data: %d, Info: %d)\n", (int)_uncompressed.size(), (int)_uncompressed.size() - _serviceInfoSize, _serviceInfoSize);
		printf("Compressed size: %d bytes\n", _compressedSize);
		printf("Compession ratio: %.2f (%.2f%%)\n", _uncompressed.empty() ? 1.f : ((float)_uncompressed.size() / (float)_compressedSize), _uncompressed.empty() ? 100 : (((float)_compressedSize / (float)_uncompressed.size()) * 100));
		printf("Final size: %d bytes\n", _compressedSize + 2);
	}

	// make text file
	std::string	_text;
	if(_saveInlFile)
	{
		char _tmp[64];
		_text = "// Generated by Packer\n";
		sprintf(_tmp, "// Num files: %d\n", _numFiles);
		_text += _tmp;
		sprintf(_tmp, "// Uncompressed size: %d bytes (Data: %d, Info: %d)\n", (int)_uncompressed.size(), (int)_uncompressed.size() - _serviceInfoSize, _serviceInfoSize);
		_text += _tmp;
		sprintf(_tmp, "// Compessed size: %d bytes\n", _compressedSize);
		_text += _tmp;
		sprintf(_tmp, "// Compession ratio: %.2f (%.2f%%)\n", _uncompressed.empty() ? 1.f : ((float)_uncompressed.size() / (float)_compressedSize), _uncompressed.empty() ? 100 : (((float)_compressedSize / (float)_uncompressed.size()) * 100));
		_text += _tmp;
		sprintf(_tmp, "// Final size: %d bytes\n", _compressedSize + 2);
		_text += _tmp;
		_text += "const unsigned char gPackedData[] = {\n";
		sprintf(_tmp, "0x%02x, 0x%02x,\n", (uint8_t)(_uncompressed.size() & 0xff), (uint8_t)((_uncompressed.size() >> 8) & 0xff));
		_text += _tmp;
		for (int i = 0, _line = 0; i < _compressedSize; ++i)
		{
			sprintf(_tmp, "0x%02x, ", (uint8_t)_compressed[i]);
			_text += _tmp;
			if (_line++ > 20)
			{
				_line = 0;
				_text += "\n";
			}
		}
		_text += "\n};\n";

		if (!SaveFile(_outputFilename, "inl", _text.c_str(), _text.length(), false))
			return -1;

	}
	else if(_saveRawFile)
	{
		if (!_compressed.empty() && !SaveFile(_outputFilename, "raw", &_compressed[0], _compressedSize, _useCompression, _uncompressed.size()))
			return -1;
	}

	if (_saveRawFileAlso)
	{
		if (!_compressed.empty() && !SaveFile((_outputFilename + std::string(".raw")).c_str(), "raw", &_compressed[0], _compressedSize, _useCompression, _uncompressed.size()))
			return -1;
	}

	return 0;
}
