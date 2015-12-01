#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "TemplatedMapManager.h"
#include "EffectTechnique.h"

#include "EffectShader.h"
#include "EffectTechnique.h"
#include "EffectParameters.h"
#include <string.h>

class CEffectManager : public CTemplatedMapManager<CEffectTechnique>
{
private:
	CTemplatedMapManager<CEffectVertexShader> m_VertexShaders;
	CTemplatedMapManager<CEffectPixelShader> m_PixelShaders;
	std::string m_Filename;
public:
	CEffectManager();
	virtual ~CEffectManager();
	static CEffectParameters m_Parameters;
	void Reload();
	bool Load(const std::string &Filename);
	CEffectVertexShader * GetVertexShader(const std::string &VertexShader);
	CEffectPixelShader * GetPixelShader(const std::string &PixelShader);
};

#endif //EFFECT_MANAGER_H