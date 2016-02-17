#ifndef DYNAMIC_TEXTURE_MANAGER_H
#define DYNAMIC_TEXTURE_MANAGER_H

#include "Utils.h"
#include "Texture.h"

class CDynamicTexture;
class CMaterial;

class CDynamicTextureManager
{
protected:
	CDynamicTexture *m_Texture;
	CMaterial *m_Material;
public:
	CDynamicTextureManager(CDynamicTexture *_m_Texture, CMaterial *_m_Material);
	~CDynamicTextureManager();
	void Destroy();
	UAB_GET_PROPERTY(CDynamicTexture*, Texture);
	UAB_GET_PROPERTY(CMaterial*, Material);
};

#endif