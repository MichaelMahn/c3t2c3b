#ifndef FBXCONV_READERS_C3TCONVERTER_H
#define FBXCONV_READERS_C3TCONVERTER_H

#include "Reader.h"
#include "cocos2d.h"

namespace fbxconv {
namespace readers {

class C3TConverter : public Reader
{
	public:
		C3TConverter() {}
		virtual ~C3TConverter() {}

		virtual bool load(Settings *settings) override {
			/*bundle = cocos2d::Bundle3D::createBundle();
			if (!bundle->load(settings->inFile)) {
				cocos2d::Bundle3D::destroyBundle(bundle);
				return false;
			}*/
			return true;
		}

		virtual bool convert(Settings *settings, Model *const &model) override {
			/*auto ret = bundle->loadMeshDatas(*meshdatas)&& bundle->loadMaterials(*materialdatas) && bundle->loadNodes(*nodedatas);
			cocos2d::Bundle3D::destroyBundle(bundle);*/

			return true;
		}
	protected:
	private:
		//Bundle3D *bundle;
};

} // namespace readers
} // namespace fbxconv

#endif // FBXCONV_READERS_C3TCONVERTER_H
