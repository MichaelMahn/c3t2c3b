#ifndef FBXCONV_READERS_C3TCONVERTER_H
#define FBXCONV_READERS_C3TCONVERTER_H

#include "Reader.h"
#include "cocos2d.h"
#include "3d/CCBundle3D.h"

namespace fbxconv {
namespace readers {

class C3TConverter : public Reader
{
	public:
		C3TConverter() {}
		virtual ~C3TConverter() {}

		virtual bool load(Settings *settings) override {
			cocos2d::Bundle3D *bundle = cocos2d::Bundle3D::createBundle();
			if (!bundle->load(settings->inFile)) {
				cocos2d::Bundle3D::destroyBundle(bundle);
				return false;
			}

			bool ret = bundle->loadMeshDatas(meshDatas)
			        && bundle->loadMaterials(materialDatas)
			        && bundle->loadNodes(nodeDatas);
			cocos2d::Bundle3D::destroyBundle(bundle);

			return ret;
		}

		virtual bool convert(Settings *settings, Model *const &model) override {
			for(cocos2d::NMaterialData const& coco_matIt : materialDatas.materials) {
				Material *conv_mat = new Material;
				conv_mat->id = coco_matIt.id;
				for(cocos2d::NTextureData const& coco_textIt : coco_matIt.textures) {
					Material::Texture *conv_text = new Material::Texture;
					conv_text->id = coco_textIt.id;
					conv_text->path = coco_textIt.filename;
					conv_text->usage = (Material::Texture::Usage) coco_textIt.type;
					conv_text->wrapModeU = (FbxTexture::EWrapMode) coco_textIt.wrapS;
					conv_text->wrapModeV = (FbxTexture::EWrapMode) coco_textIt.wrapT;
					conv_mat->textures.push_back(conv_text);
				}
				model->materials.push_back(conv_mat);
			}

			for(cocos2d::MeshData const* coco_meshIt : meshDatas.meshDatas) {
				Mesh *conv_mesh = new Mesh;
				conv_mesh->vertexSize = coco_meshIt->vertexSizeInFloat;
				for(float const& v : coco_meshIt->vertex) {
					conv_mesh->vertices.push_back(v);
				}

				for(int i = 0; i < coco_meshIt->subMeshIndices.size(); ++i) {
					MeshPart *conv_meshPart = new MeshPart;
					conv_meshPart->primitiveType = PRIMITIVETYPE_TRIANGLES;
					for(unsigned short const& s : coco_meshIt->subMeshIndices[i]) {
						conv_meshPart->indices.push_back(s);
					}
					if(coco_meshIt->subMeshIds.size() > i) {
						conv_meshPart->id = coco_meshIt->subMeshIds[i];
					}
					if(coco_meshIt->subMeshAABB.size() > i) {
						conv_meshPart->aabb[0] = coco_meshIt->subMeshAABB[i]._min.x;
						conv_meshPart->aabb[1] = coco_meshIt->subMeshAABB[i]._min.y;
						conv_meshPart->aabb[2] = coco_meshIt->subMeshAABB[i]._min.z;
						conv_meshPart->aabb[3] = coco_meshIt->subMeshAABB[i]._max.x;
						conv_meshPart->aabb[4] = coco_meshIt->subMeshAABB[i]._max.y;
						conv_meshPart->aabb[5] = coco_meshIt->subMeshAABB[i]._max.z;
					}
					conv_mesh->parts.push_back(conv_meshPart);
				}

				//conv_mesh->attributes.value = 0x11;

				/*for(cocos2d::MeshVertexAttrib const& coco_meshAtt : coco_Mesh->attributes) {

				}*/

				model->meshes.push_back(conv_mesh);
			}
			return true;
		}

	protected:
	private:
		cocos2d::MeshDatas meshDatas;
		cocos2d::MaterialDatas materialDatas;
		cocos2d::NodeDatas nodeDatas;
};

} // namespace readers
} // namespace fbxconv

#endif // FBXCONV_READERS_C3TCONVERTER_H
