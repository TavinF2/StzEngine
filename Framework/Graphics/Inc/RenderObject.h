#pragma once
#include "MeshBuffer.h"
#include "Transform.h"
#include "Material.h"
#include "TextureManager.h"
namespace StzEngine::Graphics
{ 
	class RenderObject
	{
	public: 
		void Terminate();
		
		MeshBuffer meshBuffer; // shape, vertices, indices
		Transform transform; // location/orientation :)
		Material material; // light data 
		TextureId diffuseMapId; 
		TextureId specMapId;
		TextureId normMapId; // normal texture of the object
		TextureId bumpMapId; // bump texture adds geometry offsets to the object
	};
}