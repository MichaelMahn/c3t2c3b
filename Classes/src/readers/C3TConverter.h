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

				for(cocos2d::MeshVertexAttrib const& coco_meshAtt : coco_meshIt->attribs) {
					char const* AttributeNames[] = {"UNKNOWN", "VERTEX_ATTRIB_POSITION", "VERTEX_ATTRIB_NORMAL", "VERTEX_ATTRIB_COLOR", "COLORPACKED", "VERTEX_ATTRIB_TANGENT", "VERTEX_ATTRIB_BINORMAL", "VERTEX_ATTRIB_TEX_COORD", "VERTEX_ATTRIB_TEX_COORD1", "VERTEX_ATTRIB_TEX_COORD2", "VERTEX_ATTRIB_TEX_COORD3", "VERTEX_ATTRIB_TEX_COORD4", "VERTEX_ATTRIB_TEX_COORD5", "VERTEX_ATTRIB_TEX_COORD6", "VERTEX_ATTRIB_TEX_COORD7", "VERTEX_ATTRIB_BLEND_WEIGHT", "VERTEX_ATTRIB_BLEND_INDEX", "BLENDWEIGHT2", "BLENDWEIGHT3", "BLENDWEIGHT4", "BLENDWEIGHT5", "BLENDWEIGHT6", "BLENDWEIGHT7"};
					MeshVertexAttrib conv_att;
					CCLOG("%i", coco_meshAtt.vertexAttrib);
					if(coco_meshAtt.vertexAttrib < sizeof(AttributeNames) / sizeof(char const*)) {
						conv_att.name = AttributeNames[coco_meshAtt.vertexAttrib];
					} else {
						conv_att.name = "UNKNOWN";
					}
					conv_att.size = coco_meshAtt.size;
					conv_att.type = "GL_FLOAT";
					conv_mesh->attributes.attributemap[conv_att.name] = conv_att;
					//coco_meshAtt.vertexAttrib
					conv_mesh->attributes.set(coco_meshAtt.vertexAttrib, true);
				}

				CCLOG("%i", conv_mesh->attributes.size());

				model->meshes.push_back(conv_mesh);
			}

			for(cocos2d::NodeData const* coco_node : nodeDatas.nodes) {
				model->nodes.push_back(loadNode(*coco_node, false, model));
			}
			for(cocos2d::NodeData const* coco_node : nodeDatas.skeleton) {
				model->nodes.push_back(loadNode(*coco_node, true, model));
			}
			return true;
		}

	protected:
	private:
		Node* loadNode(cocos2d::NodeData const& pNode, bool skeleton, Model *const &model) {
			Node *conv_node = new Node;
			conv_node->id = pNode.id;
			memcpy(conv_node->transforms, pNode.transform.m, sizeof(conv_node->transforms));
			conv_node->_skeleton = skeleton;
			for(cocos2d::NodeData const* coco_node : pNode.children) {
				conv_node->children.push_back(loadNode(*coco_node, skeleton, model));
			}
			for(cocos2d::ModelData const* coco_model : pNode.modelNodeDatas) {
				NodePart *conv_nodePart = new NodePart;
				std::vector<Material*>::iterator material = std::find_if(model->materials.begin(),
				                                                         model->materials.end(),
				                                                         [&coco_model](Material* const& mat){return mat->id == coco_model->matrialId;});
				if(material != model->materials.end()) {
					conv_nodePart->material = *material;
				} else {
					conv_nodePart->material = nullptr;
				}

				conv_nodePart->meshPart = nullptr;
				for(Mesh const* conv_mesh : model->meshes) {
					std::vector<MeshPart*>::const_iterator meshPart = std::find_if(conv_mesh->parts.begin(),
					                                                               conv_mesh->parts.end(),
					                                                               [&coco_model](MeshPart* const& meshPart){return meshPart->id == coco_model->subMeshId;});
					if(meshPart != conv_mesh->parts.end()) {
						conv_nodePart->meshPart = *meshPart;
						break;
					}
				}
				conv_node->parts.push_back(conv_nodePart);
			}
			return conv_node;
		}

		cocos2d::MeshDatas meshDatas;
		cocos2d::MaterialDatas materialDatas;
		cocos2d::NodeDatas nodeDatas;
};

} // namespace readers
} // namespace fbxconv

#endif // FBXCONV_READERS_C3TCONVERTER_H
