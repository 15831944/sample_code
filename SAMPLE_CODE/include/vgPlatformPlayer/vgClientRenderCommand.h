#include <vgKernel/vgkRenderCommand.h>
#include <vgKernel/vgkRendererManager.h>
#include <vgMesh/vgmMesh.h>

class ClientRenderMngRender : public vgKernel::RenderCommand
{
public:
	ClientRenderMngRender() :  vgKernel::RenderCommand(VG_RP_CLIENT_RENDERMNG_RENDER) {}

	// Ϊ���Ч��д��inline��ʵ�ֿɷ���CPP�ļ�
	inline virtual bool render()
	{
		
		return true;
	}
};
