#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif//_CRT_SECURE_NO_WARNINGS

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "..\Engine\Core.hpp"
#include "..\Engine\File.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <vector>
#include <math.h>

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

//The Blender loader does not support animations yet: http://www.assimp.org/lib_html/importer_notes.html	

// http://iquilezles.org/www/articles/meshcompression/meshcompression.htm

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

uint String::Hash(const char* _str, uint _hash)
{
	if (!_str)
		_str = "";
	while (*_str)
		_hash = ((_hash >> 1) + ((_hash & 1) << 15) + Lower(*_str++)) & 0xffff;
	return _hash;
}

bool Equals(float _lhs, float _rhs, float _error = 0)
{
	return abs(_lhs - _rhs) <= _error;
}

uint16 FloatToHalf(float _value)
{
	union { float f; uint32 i; }_fb = { _value };
#if 0
	return (uint16)((_fb.i >> 16) & 0x8000) | ((((_fb.i & 0x7f800000) - 0x38000000) >> 13) & 0x7c00) | ((_fb.i >> 13) & 0x03ff);
#else
	uint32 _s = (_fb.i >> 16) & 0x00008000; // sign
	int32 _e = ((_fb.i >> 23) & 0x000000ff) - 0x00000070; // exponent
	uint32 _r = _fb.i & 0x007fffff; // mantissa
	if (_e < 1)
	{
		if (_e < -10)
			return 0;
		_r = (_r | 0x00800000) >> (14 - _e);
		return (uint16)(_s | _r);
	}
	else if (_e == 0x00000071)
	{
		if (_r == 0)
			return (uint16)(_s | 0x7c00); // Inf
		else
			return (uint16)(((_s | 0x7c00) | (_r >>= 13)) | (_r == 0)); // NAN
	}
	if (_e > 30)
		return (uint16)(_s | 0x7c00); // Overflow
	return (uint16)((_s | (_e << 10)) | (_r >> 13));
#endif
}

float HalfToFloat(uint16 _value)
{
	union { uint32 i; float f; }_fb;
#if 0
	_fb.i = ((_value & 0x8000) << 16) | (((_value & 0x7c00) + 0x1C000) << 13) | ((_value & 0x03FF) << 13);
#else
	int32 _s = (_value >> 15) & 0x00000001; // sign
	int32 _e = (_value >> 10) & 0x0000001f; // exponent
	int32 _r = _value & 0x000003ff; // mantissa
	if (_e == 0)
	{
		if (_r == 0) // Plus or minus zero
		{
			_fb.i = _s << 31;
			return _fb.f;
		}
		else // Denormalized number -- renormalize it
		{
			for (; !(_r & 0x00000400); _r <<= 1, _e -= 1);
			_e += 1;
			_r &= ~0x00000400;
		}
	}
	else if (_e == 31)
	{
		if (_r == 0) // Inf
		{
			_fb.i = (_s << 31) | 0x7f800000;
			return _fb.f;
		}
		else // NaN
		{
			_fb.i = ((_s << 31) | 0x7f800000) | (_r << 13);
			return _fb.f;
		}
	}
	_e = (_e + 112) << 23;
	_r = _r << 13;
	_fb.i = ((_s << 31) | _e) | _r;
#endif
	return _fb.f;
}

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

struct OutFile
{
	OutFile&  Write(const void* _data, uint _size)
	{
		for (const uint8* s = reinterpret_cast<const uint8*>(_data); _size--; )
			data.push_back(*s++);
		return *this;
	}
	OutFile&  WriteAt(uint _offset, const void* _data, uint _size)
	{
		for (const uint8* s = reinterpret_cast<const uint8*>(_data); _size--; )
			data[_offset++] = *s++;
		return *this;
	}

	template <class T> OutFile& Write(T _value)
	{
		return Write(&_value, sizeof(_value));
	}

	template <class T> OutFile& WriteAt(uint _offset, T _value)
	{
		return WriteAt(_offset, &_value, sizeof(_value));
	}

	uint Skip(uint _num)
	{
		uint _pos = (uint)data.size();
		while (_num--)
			data.push_back(0);
		return _pos;
	}

	uint Pos(void)
	{
		return (uint)data.size();
	}

	bool Save(const char* _filename)
	{
		FILE* _f = fopen(_filename, "wb");
		if (!_f)
		{
			printf("Couldn't create file \"%s\"\n", _filename);
			return false;
		}

		printf("Save to \"%s\"\n", _filename);

		fwrite(data.data(), 1, data.size(), _f);
		fclose(_f);

		return true;
	}

	std::vector<uint8> data;
};

//----------------------------------------------------------------------------//
// Converter
//----------------------------------------------------------------------------//

struct Converter
{
	const aiScene* scene = nullptr;
	OutFile file;
	float maxError = 0.005f; // .002f;
	bool useIdenticalPrecision = true;

	bool ProcessFile(const char* _filename)
	{
		Assimp::Importer _importer;
		uint _flags =
			//aiProcess_Triangulate |
			//aiProcess_SortByPType | 
			aiProcess_JoinIdenticalVertices |
			aiProcess_FindInstances |
			aiProcess_OptimizeMeshes |
			aiProcess_RemoveRedundantMaterials |
			aiProcess_GenSmoothNormals |
			//aiProcess_GenNormals |
			aiProcess_FixInfacingNormals;
			//aiProcess_RemoveComponent;
			// aiProcess_OptimizeGraph;

		scene = _importer.ReadFile(_filename, _flags);

		if (!scene)
		{
			printf("Import error: %s\n", _importer.GetErrorString());
			return false;
		}

		printf("File: \"%s\", %d meshes, %d materials, %d root nodes, %d animations\n", 
			_filename, 
			scene->mNumMeshes,
			scene->mNumMaterials,
			scene->mRootNode ? scene->mRootNode->mNumChildren : 0,
			scene->mNumAnimations);

		// header

		uint _sizeOffset = file.Skip(2);
		uint _start = file.Pos();
		{
			file.Write<uint16>(String::Hash(_filename)); // write name, 2 byte
			file.Write<uint8>(scene->mNumMeshes); // write num meshes, 1 byte
		}

		// hierarchy
		if(scene->mRootNode)
		{
			if (scene->mRootNode->mNumChildren > 0xff)
			{
				printf("Error: Too many nodes (%d, 255 max)\n", scene->mRootNode->mNumChildren);
				return false;
			}
			file.Write<uint8>(scene->mRootNode->mNumChildren); // write count, 1 byte

			for (uint i = 0; i < scene->mRootNode->mNumChildren; ++i)
			{
				if (!WriteNode(scene->mRootNode->mChildren[i]))
					return false;
			}
		}
		else
		{
			file.Write<uint8>(0); // write count, 1 byte
		}

		// materials
		{
			if (scene->mNumMeshes > 0xff)
			{
				printf("Error: Too many materials (%d, 255 max)\n", scene->mNumMaterials);
				return false;
			}
			file.Write<uint8>(scene->mNumMaterials); // write count, 1 byte

			for (uint i = 0; i < scene->mNumMaterials; ++i)
			{
				if (!WriteMaterial(scene->mMaterials[i]))
					return false;
			}
		}

		// meshes
		{
			if (scene->mNumMeshes > 0xff)
			{
				printf("Error: Too many meshes (%d, 255 max)\n", scene->mNumMeshes);
				return false;
			}

			aiVector3D _min(99999.f);
			aiVector3D _max(-99999.f);

			for (uint i = 0; i < scene->mNumMeshes; ++i)
			{
				const aiMesh* _mesh = scene->mMeshes[i];
				for (uint j = 0; j < _mesh->mNumVertices; ++j)
				{
					const aiVector3D& _p = _mesh->mVertices[j];

					if (_p.x < _min.x)
						_min.x = _p.x;
					if (_p.y < _min.y)
						_min.y = _p.y;
					if (_p.z < _min.z)
						_min.z = _p.z;
					if (_p.x > _max.x)
						_max.x = _p.x;
					if (_p.y > _max.y)
						_max.y = _p.y;
					if (_p.z > _max.z)
						_max.z = _p.z;
				}
			}

			for (uint i = 0; i < scene->mNumMeshes; ++i)
			{
				if (!WriteMesh(scene->mMeshes[i], _min, _max, useIdenticalPrecision))
					return false;
			}
		}


		// animations
		{
			if (scene->mNumAnimations > 0xff)
			{
				printf("Error: Too many animations (%d, 255 max)\n", scene->mNumAnimations);
				return false;
			}
			file.Write<uint8>(scene->mNumAnimations); // write count, 1 byte

			for (uint i = 0; i < scene->mNumAnimations; ++i)
			{
				if (!WriteAnim(scene->mAnimations[i]))
					return false;
			}
		}

		file.WriteAt<uint16>(_sizeOffset, file.Pos() - _start);

		return true;
	}

	bool WriteMaterial(const aiMaterial* _material)
	{
		return true;
	}

	bool WriteMesh(const aiMesh* _mesh, aiVector3D _min, aiVector3D _max, bool _identicalPrecision)
	{
		printf("Mesh \"%s\" (0x%04x)", _mesh->mName.C_Str(), String::Hash(_mesh->mName.C_Str()));
		printf(", %d vertices", _mesh->mNumVertices);
		printf(", %d faces", _mesh->mNumFaces);
		
		uint _start = file.Pos();

		// bounds, offset and scale
		aiVector3D _diff, _mul, _add, _sub, _div;
		{

			if (!_identicalPrecision)
			{
				_min = _mesh->mVertices[0];
				_max = _mesh->mVertices[0];
				for (uint i = 0; i < _mesh->mNumVertices; ++i)
				{
					const aiVector3D& _p = _mesh->mVertices[i];

					if (_p.x < _min.x)
						_min.x = _p.x;
					if (_p.y < _min.y)
						_min.y = _p.y;
					if (_p.z < _min.z)
						_min.z = _p.z;
					if (_p.x > _max.x)
						_max.x = _p.x;
					if (_p.y > _max.y)
						_max.y = _p.y;
					if (_p.z > _max.z)
						_max.z = _p.z;
				}
			}

			 _diff = _max - _min;
			 _mul = _diff;
			 _add = _min;
			 _sub = _min;
			 _div = _diff;
			
			if (_div.x == 0)
				_div.x = 1;
			if (_div.y == 0)
				_div.y = 1;
			if (_div.z == 0)
				_div.z = 1;
		}


		// bits per vertex
		int _bitsPerVertex[3];
		{
			for (uint i = 0; i < 3; ++i)
			{
				_bitsPerVertex[i] = _diff[i] > 0 ? (int)log2(_diff[i] / maxError) : 1;
				if (_bitsPerVertex[i] <= 0)
					_bitsPerVertex[i] = 1;
				//printf(" %d", _bitsPerVertex[i]);
			}
			printf(", %d{%d %d %d} bits per vertex", _bitsPerVertex[0] + _bitsPerVertex[1] + _bitsPerVertex[2], _bitsPerVertex[0], _bitsPerVertex[1], _bitsPerVertex[2]);
		}
		uint _maxValue[3] =
		{
			((uint)-1) >> (32 - _bitsPerVertex[0]),
			((uint)-1) >> (32 - _bitsPerVertex[1]),
			((uint)-1) >> (32 - _bitsPerVertex[2]),
		};
		//printf("{%d %d %d}", _maxValue[0], _maxValue[1], _maxValue[2]);

		// bits per index;
		int _bitsPerIndex;
		{
			_bitsPerIndex = (uint)ceil(log2(_mesh->mNumVertices));
			printf(", %d bits per index", _bitsPerIndex);
		}

		printf(", %d bones", _mesh->mNumBones);


		aiString _mtlName;
		scene->mMaterials[_mesh->mMaterialIndex]->Get(AI_MATKEY_NAME, _mtlName);
		printf(", material \"%s\" (0x%04x)", _mtlName.C_Str(), String::Hash(_mtlName.C_Str()));
		printf("\n");


		// write size, 2 byte
		//uint _sizeOffset = file.Skip(2);
		//uint _start = file.Pos();

		// header, 27 byte
		{
			file.Write<uint16>(String::Hash(_mesh->mName.C_Str())); // name, 2 byte
			file.Write<uint8>(_mesh->mMaterialIndex); // material index, 1 byte
			file.Write(_mul); // mul, 12 byte
			file.Write(_add); // add, 12 byte
		}

		// write vertices

		std::vector<uint> _uniqueVertices;
		{
			BitStream<OutFile> _out(&file);

			_out.Write(_mesh->mNumVertices, 16); // write count

			if (_mesh->mNumVertices) // write bits
			{
				_out.Write(_bitsPerVertex[0], 8); // bits per x, 1 byte
				_out.Write(_bitsPerVertex[1], 8); // bits per y, 1 byte
				_out.Write(_bitsPerVertex[2], 8); // bits per z, 1 byte
			}

			for (uint j = 0; j < _mesh->mNumVertices; ++j)
			{
				const aiVector3D& _pos = _mesh->mVertices[j];
				bool _mirror = false;
				bool _duplicate = false;

				uint _src = 0;
				for (uint i = 0; i < (uint)_uniqueVertices.size(); ++i)
				{
					const aiVector3D& _u = _mesh->mVertices[_uniqueVertices[i]];
					if (Equals(_pos.y, _u.y, 0) && Equals(_pos.z, _u.z, 0))
					{
						if (Equals(_pos.x, _u.x, 0))
						{
							_duplicate = true;
							break;
						}
						if (Equals(abs(_pos.x), abs(_u.x), 0))
						{
							_mirror = true;
							break;
						}
					}
					++_src;
				}

				_out.Write(_duplicate, 1);
				_out.Write(_mirror, 1);

				if (_mirror || _duplicate) // write link
				{
					uint _bits = (uint)ceil(log2(_uniqueVertices.size() | 1));
					_out.Write(_src, _bits);
				}
				else // write vertex
				{
					_uniqueVertices.push_back(j);

					for (uint i = 0; i < 3; ++i)
						_out.Write((uint)(((_pos[i] - _sub[i]) / _div[i]) * _maxValue[i]), _bitsPerVertex[i]);
				}
			}

			_out.WriteLastByte();
		}
		printf("\t%d unique vertices (%d bit max index)\n", _uniqueVertices.size(), (uint)ceil(log2(_uniqueVertices.size() | 1)));

		// write color set

		std::vector<uint> _uniqueColors;
		if (_mesh->mColors[0] && true)
		{
			BitStream<OutFile> _out(&file);

			uint _bitsPerChannel = 4;
			uint _maxValue = ((uint)-1) >> (32 - _bitsPerChannel);
			float _epsilon = 1.f / _maxValue;

			_out.Write(_bitsPerChannel, 8); // write bits, 1 byte

			for (uint j = 0; j < _mesh->mNumVertices; ++j)
			{
				const aiColor4D& _color = _mesh->mColors[0][j];
				bool _duplicate = false;

				uint _src = 0;
				for (uint i = 0; i < (uint)_uniqueColors.size(); ++i)
				{
					const aiColor4D& _u = _mesh->mColors[0][_uniqueColors[i]];
					if (Equals(_color.r, _u.r, _epsilon) &&
						Equals(_color.g, _u.g, _epsilon) &&
						Equals(_color.b, _u.b, _epsilon) &&
						Equals(_color.a, _u.a, _epsilon))
					{
						_duplicate = true;
						break;
					}
					++_src;
				}

				_out.Write(_duplicate, 1);

				if (_duplicate) // write link
				{
					uint _bits = (uint)ceil(log2(_uniqueColors.size() | 1));
					_out.Write(_src, _bits);
				}
				else // write color
				{
					_uniqueColors.push_back(j);

					for (uint i = 0; i < 4; ++i)
						_out.Write((uint)(_color[i] * _maxValue), _bitsPerChannel);
				}
			}

			_out.WriteLastByte();
		}
		else
		{
			BitStream<OutFile> _out(&file);
			_out.Write(0, 8); // write bits, 1 byte
			_out.WriteLastByte();
		}
		printf("\t%d unique colors (%d bit max index)\n", _uniqueColors.size(), (uint)ceil(log2(_uniqueColors.size() | 1)));

		// write uv set

		std::vector<uint> _uniqueTexCoords;
		if (_mesh->mTextureCoords[0] && true)
		{
			BitStream<OutFile> _out(&file);

			uint _bitsPerChannel = 8;
			uint _maxValue = ((uint)-1) >> (32 - _bitsPerChannel);
			float _epsilon = 1.f / _maxValue;

			_out.Write(_bitsPerChannel, 8); // write bits, 1 byte

			for (uint j = 0; j < _mesh->mNumVertices; ++j)
			{
				const aiVector3D& _tc = _mesh->mTextureCoords[0][j];
				bool _duplicate = false;

				uint _src = 0;
				for (uint i = 0; i < (uint)_uniqueTexCoords.size(); ++i)
				{
					const aiVector3D& _u = _mesh->mTextureCoords[0][_uniqueTexCoords[i]];
					if (Equals(_tc.x, _u.x, _epsilon) &&
						Equals(_tc.y, _u.y, _epsilon))
					{
						_duplicate = true;
						break;
					}
					++_src;
				}

				_out.Write(_duplicate, 1);

				if (_duplicate) // write link
				{
					uint _bits = (uint)ceil(log2(_uniqueTexCoords.size() | 1));
					_out.Write(_src, _bits);
				}
				else // write texcoord
				{
					_uniqueTexCoords.push_back(j);

					for (uint i = 0; i < 2; ++i)
						_out.Write((uint)(_tc[i] * _maxValue), _bitsPerChannel);
				}
			}

			_out.WriteLastByte();
		}
		else
		{
			BitStream<OutFile> _out(&file);
			_out.Write(0, 8); // write bits, 1 byte
			_out.WriteLastByte();
		}
		printf("\t%d unique uv's (%d bit max index)\n", _uniqueTexCoords.size(), (uint)ceil(log2(_uniqueTexCoords.size() | 1)));

		// write faces
		{
			BitStream<OutFile> _out(&file);

			_out.Write(_mesh->mNumFaces, 16); // write count, 2 byte
			
			/*if (_mesh->mNumFaces)
			{
				_out.Write(_bitsPerIndex, 8); // write bits, 1 byte
			}*/

			for (uint i = 0; i < _mesh->mNumFaces; ++i)
			{
				const aiFace& _face = _mesh->mFaces[i];
				if (_face.mNumIndices != 3 && _face.mNumIndices != 4)
				{
					printf("Error: Face #%d is not triangle or quad (%d indices)\n", i, _face.mNumIndices);
					return false;
				}
				_out.Write(_face.mNumIndices == 4, 1); // 3 or 4
				_out.Write(_face.mIndices[0], _bitsPerIndex);
				_out.Write(_face.mIndices[1], _bitsPerIndex);
				_out.Write(_face.mIndices[2], _bitsPerIndex);

				if (_face.mNumIndices == 4)
					_out.Write(_face.mIndices[3], _bitsPerIndex);
			}
			_out.WriteLastByte();
		}

		// write bones
		{
			BitStream<OutFile> _out(&file);
			uint _bits = 5;
			uint _maxValue = ((uint)-1) >> (32 - _bits);

			_out.Write(_mesh->mNumBones, 8); // write count, 1 byte

			if (_mesh->mNumBones)
			{
				if (_mesh->mNumBones > 0xff)
				{
					printf("\t Error: Too many bones (%d, max 255)\n", _mesh->mNumBones);
				}

				_out.Write(_bits, 8); // write bits, 1 byte
			}

			for (uint j = 0; j < _mesh->mNumBones; ++j)
			{
				const aiBone* _bone = _mesh->mBones[j];
				printf("\tBone \"%s\" (0x%04x), %d weights\n", _bone->mName.C_Str(), String::Hash(_bone->mName.C_Str()), _bone->mNumWeights);

				_out.Write(String::Hash(_bone->mName.C_Str()), 16);
				_out.Write(_bone->mNumWeights, _bitsPerIndex + 1);
				
				//WriteTransform(_out, _bone->mOffsetMatrix); // restore from a hierarchy?

				for (uint i = 0; i < _bone->mNumWeights; ++i)
				{
					const aiVertexWeight& _weight = _bone->mWeights[i];
					_out.Write(_weight.mVertexId, _bitsPerIndex);
					_out.Write((uint)(_weight.mWeight * _maxValue), _bits);
				}
			}
			_out.WriteLastByte();
		}

		//file.WriteAt<uint16>(_sizeOffset, file.Pos() - _start); // write size at strat, 2 byte

		printf("\tSize: %d bytes\n", file.Pos() - _start);
		return true;
	}

	bool WriteNode(const aiNode* _node)
	{
		for (const aiNode* i = _node->mParent; i && i->mParent; i = i->mParent)
			printf("\t");
		printf("Node \"%s\" (0x%04x), %d meshes, %d children\n", _node->mName.C_Str(), String::Hash(_node->mName.C_Str()), _node->mNumMeshes, _node->mNumChildren);

		if (_node->mNumMeshes > 0xff)
		{
			printf("Error: Too many meshes (%d, 255 max)\n", _node->mNumMeshes);
			return false;
		}
		if (_node->mNumChildren > 0xff)
		{
			printf("Error: Too many children (%d, 255 max)\n", _node->mNumChildren);
			return false;
		}

		file.Write<uint16>(String::Hash(_node->mName.C_Str())); // write name, 2 byte

		// write transform
		{
			BitStream<OutFile> _out(&file);
			WriteTransform(_out, _node->mTransformation);
			_out.WriteLastByte();
		}
		
		// TODO: write metadata

		file.Write<uint8>(_node->mNumMeshes); // write num meshes, 1 byte
		for (uint i = 0; i < _node->mNumMeshes; ++i)
		{
			file.Write<uint8>(_node->mMeshes[i]); // write mesh index, 1 byte

			for (const aiNode* i = _node; i && i->mParent; i = i->mParent)
				printf("\t");
			printf("Mesh \"%s\" (0x%04x)\n", scene->mMeshes[_node->mMeshes[i]]->mName.C_Str(), String::Hash(scene->mMeshes[_node->mMeshes[i]]->mName.C_Str()));
		}


		file.Write<uint8>(_node->mNumChildren); // write num childen, 1 byte
		for (uint i = 0; i < _node->mNumChildren; ++i)
		{
			WriteNode(_node->mChildren[i]); // write child hierarchy
		}

		return true;
	}

	bool WriteAnim(const aiAnimation* _anim)
	{
		printf("Anim \"%s\" (0x%04x), %d channels, %.1f fps\n", _anim->mName.C_Str(), String::Hash(_anim->mName.C_Str()), _anim->mNumChannels, _anim->mTicksPerSecond);
		
		if (_anim->mNumChannels > 0xff)
		{
			printf("Error: Too many channels (%d, 255 max)\n", _anim->mNumChannels);
			return false;
		}

		file.Write<uint16>(String::Hash(_anim->mName.C_Str())); // write name, 2 byte
		file.Write<uint8>(_anim->mNumChannels); // write count, 1 byte

		for (uint i = 0; i < _anim->mNumChannels; ++i)
		{
			WriteAnimChannel(_anim->mChannels[i]);
		}
		
		return true;
	}

	bool WriteAnimChannel(const aiNodeAnim* _anim)
	{
		printf("\tChannel \"%s\" (0x%04x)", _anim->mNodeName.C_Str(), String::Hash(_anim->mNodeName.C_Str()));

		file.Write<uint16>(String::Hash(_anim->mNodeName.C_Str())); // node name, 2 byte

		// position keys
		if(1)
		{
			file.Write<uint16>(_anim->mNumPositionKeys); // write count, 1 byte

			uint _start = file.Pos();
			BitStream<OutFile> _out(&file);

			for (uint i = 0; i < _anim->mNumPositionKeys; ++i)
			{
				_out.Write(FloatToHalf((float)_anim->mPositionKeys[i].mTime), 16); // write time, 16 bit
				WriteTranslation(_out, _anim->mPositionKeys[i].mValue); // write value
			}

			_out.WriteLastByte();

			printf(", %d position keys (%d bytes)", _anim->mNumPositionKeys, file.Pos() - _start);
		}
		else
		{
			file.Write<uint16>(0); // write count, 1 byte
		}

		// rotation keys
		if (1)
		{
			file.Write<uint16>(_anim->mNumRotationKeys); // write count, 1 byte

			uint _start = file.Pos();
			BitStream<OutFile> _out(&file);

			for (uint i = 0; i < _anim->mNumRotationKeys; ++i)
			{
				_out.Write(FloatToHalf((float)_anim->mRotationKeys[i].mTime), 16); // write time, 16 bit
				WriteRotation(_out, _anim->mRotationKeys[i].mValue); // write value
			}

			_out.WriteLastByte();
			printf(", %d rotation keys (%d bytes)", _anim->mNumRotationKeys, file.Pos() - _start);
		}
		else
		{
			file.Write<uint16>(0); // write count, 1 byte
		}

		// scale keys
		if(0)
		{
			file.Write<uint16>(_anim->mNumRotationKeys); // write count, 1 byte

			uint _start = file.Pos();
			BitStream<OutFile> _out(&file);

			for (uint i = 0; i < _anim->mNumScalingKeys; ++i)
			{
				_out.Write(FloatToHalf((float)_anim->mScalingKeys[i].mTime), 16); // write time, 16 bit
				WriteScale(_out, _anim->mScalingKeys[i].mValue); // write value
			}
			_out.WriteLastByte();
			printf(", %d scale keys (%d bytes)", _anim->mNumScalingKeys, file.Pos() - _start);
		}
		else
		{
			file.Write<uint16>(0); // write count, 1 byte
		}

		printf("\n");

		return true;
	}

	void WriteTranslation(BitStream<OutFile>& _stream, const aiVector3D& _pos) // 1 - 49 bit
	{
		if (_pos.x == 0 && _pos.y == 0 && _pos.z == 0)
		{
			_stream.Write(1, 1);
		}
		else
		{
			_stream.Write(0, 1);
			_stream.Write(FloatToHalf(_pos.x), 16);
			_stream.Write(FloatToHalf(_pos.y), 16);
			_stream.Write(FloatToHalf(_pos.z), 16);
		}
	}
	
	void WriteRotation(BitStream<OutFile>& _stream, const aiQuaternion& _rot) // 1 - 65 bit
	{
		if (_rot.x == 0 && _rot.y == 0 && _rot.z == 0 && _rot.w == 1) // identity, 1 bit
		{
			_stream.Write(1, 1);
		}
		else // 65 bit
		{
			// TODO: use euler angles
			_stream.Write(0, 1);
			_stream.Write(FloatToHalf(_rot.x), 16);
			_stream.Write(FloatToHalf(_rot.y), 16);
			_stream.Write(FloatToHalf(_rot.z), 16);
			_stream.Write(FloatToHalf(_rot.w), 16);
		}
	}

	void WriteScale(BitStream<OutFile>& _stream, const aiVector3D& _scl) // 2 - 49 bit
	{
		if (_scl.x == _scl.y && _scl.x == _scl.z) // unitform, 2-17 bit
		{
			_stream.Write(1, 1);
			if (_scl.x == 1)
			{
				_stream.Write(1, 1);
			}
			else
			{
				_stream.Write(0, 1);
				_stream.Write(FloatToHalf(_scl.x), 16);
			}
		}
		else // 49 bit
		{
			_stream.Write(1, 0);
			_stream.Write(FloatToHalf(_scl.x), 16);
			_stream.Write(FloatToHalf(_scl.y), 16);
			_stream.Write(FloatToHalf(_scl.z), 16);
		}
	}

	void WriteTransform(BitStream<OutFile>& _stream, const aiMatrix4x4& _m)	// 4 - 163 bit (1 - 21 byte) 
	{
		aiVector3D _pos, _scl;
		aiQuaternion _rot;
		_m.Decompose(_scl, _rot, _pos);
		
		WriteTranslation(_stream, _pos);
		WriteRotation(_stream, _rot);
		WriteScale(_stream, _scl);
	}
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

int main(int _argc, const char** _argv)
{
	// skip exe filename
	if (_argc)
		_argc--, _argv++;

	Converter _conv;

	while (**_argv == '/' && _argc > 0)
	{
		const char* _opt = *_argv++;
		_argc--;
		if (!strcmp(_opt, "/tc")) 
		{
			
		}
		else
		{
			// TODO: more options

			printf("Unknown option '%s'\n", _opt);
			printf("Supported options: /tc\n");
			return 0;
		}
	}

	// no args
	if (_argc < 1)
	{
		printf("Usage: /options output file1, file2 ...\n");
		//printf("/r - save raw file as output (default)\n");
		return 0;
	}

	// output filename
	const char* _outputFilename = *_argv++;
	--_argc;

	while (_argc--)
	{
		if (!_conv.ProcessFile(*_argv++))
			return -1;
	}

	printf("Final size: %d bytes\n", _conv.file.Pos());
	_conv.file.Save(_outputFilename);

	return 0;
}