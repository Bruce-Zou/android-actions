#include <string.h>
#include "buffer_mng.h"
#include "omx_malloc.h"
#include "video_mediadata.h"
#include "Actbuffer.h"
#include "omx_comp_debug_levels.h"
#include "log.h"

static void Printf_BuffersMng_List(OMX_VCE_Buffers_List*pBuffersMng_List)
{
	unsigned int i;
	OMX_VCE_Buffer_Info* pList = pBuffersMng_List->pList;

	DEBUG(DEB_LEV_PARAMS, "Printf_List,Actual_Buffers:%ld\n", pBuffersMng_List->Actual_Buffers);

	for (i = 0; i < MAX_VCE_MNG_Buffers; i++)
	{
		DEBUG(DEB_LEV_PARAMS, "Printf_List,i:%d,IsValid:%lu,InPutAddr:%p,PhyAddr:%p,VirAddr:%p,Ion_Size:%ld\n", i,
				pList[i].IsValid, pList[i].InPutAddr, pList[i].PhyAddr, pList[i].VirAddr, pList[i].Ion_Size);
	}
}

OMX_ERRORTYPE FlushProcessingBuffers_BuffersMng(OMX_VCE_Buffers_List*pBuffersMng_List,OMX_BUFFERHEADERTYPE*pBuffHead,int IsStoreMedia)
{
	OMX_ERRORTYPE eError = OMX_ErrorNone;
	OMX_PTR input_addr = NULL;
	OMX_PTR phy_addr = NULL;
	OMX_PTR vir_addr = NULL;
	OMX_VCE_Buffer_Info* pList = pBuffersMng_List->pList;
	unsigned int isExist = 0;
	unsigned int i;

	if (pBuffersMng_List->Actual_Buffers < 0)
	{
		DEBUG(DEB_LEV_ERR, "err!BuffersMng_List is empty!%s,%d\n",__FILE__,__LINE__);
		return OMX_ErrorInsufficientResources;
	}

	if (IsStoreMedia)
	{
#ifdef enable_gralloc
		eError = VCE_OSAL_GetPhyAddr( (OMX_PTR)(((video_metadata_t*)(pBuffHead->pBuffer))->handle),&input_addr);
		if(eError != OMX_ErrorNone)
		{
			DEBUG(DEB_LEV_ERR, "err!VCE_OSAL_GetPhyAddr fail!%s,%d\n",__FILE__,__LINE__);
			return eError;
		}
#else
		input_addr = (OMX_PTR)(((video_handle_t*) ((video_metadata_t*) (pBuffHead->pBuffer))->handle)->phys_addr);
#endif
	}
	else
	{
		input_addr = pBuffHead->pBuffer;
	}

	/*����*/
	for (i = 0; i < MAX_VCE_MNG_Buffers; i++)
	{
		if (pList[i].IsValid == OMX_TRUE)
		{
			if (pList[i].InPutAddr == input_addr)
			{
				/*����*/
				phy_addr = pList[i].PhyAddr;
				vir_addr = pList[i].VirAddr;

				/*���*/
				pList[i].IsValid = OMX_FALSE;
				pList[i].InPutAddr = NULL;
				pList[i].PhyAddr = NULL;
				pList[i].VirAddr = NULL;
				pList[i].Ion_Size = 0;
				pBuffersMng_List->Actual_Buffers--;

				isExist = 1;
				break;
			}
		}
	}

	if (isExist == 0)
		DEBUG(DEB_LEV_ERR, "FlushProcessingBuffers,Warning!can not find the buffer\n");

	if (IsStoreMedia == OMX_TRUE)
	{
		/*�ͷ�ӳ�������ַ*/
#ifdef enable_gralloc
		if(vir_addr)
		{
			eError = VCE_OSAL_UnlockANBHandle((OMX_PTR)(((video_metadata_t *)(pBuffHead->pBuffer))->handle));
			if(eError != OMX_ErrorNone)
			{
				DEBUG(DEB_LEV_ERR, "err!VCE_OSAL_UnlockANBHandle fail!%s,%d\n",__FILE__,__LINE__);
				return eError;
			}
		}
#else
		if (vir_addr)
		{
			eError = (OMX_ERRORTYPE)omx_munmap_ion_fd(vir_addr, ((video_handle_t*) ((video_metadata_t *) (pBuffHead->pBuffer))->handle)->size);
			if(eError != OMX_ErrorNone)
			{
				DEBUG(DEB_LEV_ERR, "err!omx_munmap_ion_fd fail!%s,%d\n",__FILE__,__LINE__);
				return OMX_ErrorUndefined;
			}
		}
#endif

		DEBUG(DEB_LEV_PARAMS, "FlushProcessingBuffers,bStoreMetaData!input_addr:%p,phy_addr:%p,vir_addr:%p\n",input_addr,phy_addr,vir_addr);
	}
	else
	{
		DEBUG(DEB_LEV_PARAMS, "FlushProcessingBuffers,no bStoreMetaData!input_addr:%p,phy_addr:%p,vir_addr:%p\n",input_addr,phy_addr,vir_addr);
	}

	return OMX_ErrorNone;
}

OMX_ERRORTYPE Add_AllocateBuffer_BuffersMng(OMX_VCE_Buffers_List*pBuffersMng_List,OMX_BUFFERHEADERTYPE*pBuffHead,int IsStoreMedia,int nSizeBytes)
{
	OMX_ERRORTYPE eError = OMX_ErrorNone;
	OMX_PTR input_addr = NULL;
	OMX_PTR phy_addr = NULL;
	unsigned long vir_addr = 0;
	OMX_VCE_Buffer_Info* pList = pBuffersMng_List->pList;
	unsigned int isExist = 0;
	unsigned int i;

	//DEBUG(DEB_LEV_PARAMS, "Add_AllocateBuffer,Actual_Buffers:%d\n",pBuffersMng_List->Actual_Buffers);
	if (pBuffersMng_List->Actual_Buffers >= MAX_VCE_MNG_Buffers)
	{
		DEBUG(DEB_LEV_ERR, "err!BuffersMng_List is full!%s,%d\n",__FILE__,__LINE__);
		return OMX_ErrorInsufficientResources;
	}

	/*���������ڴ�*/
	phy_addr = omx_malloc_phy(nSizeBytes, &vir_addr);
	if (phy_addr == NULL)
	{
		DEBUG(DEB_LEV_ERR, "err!BuffersMng_List malloc fail!%s,%d\n",__FILE__,__LINE__);
		return OMX_ErrorInsufficientResources;
	}

	if (IsStoreMedia)
	{
#ifndef enable_gralloc
		/*�����ڴ�*/
		pBuffHead->pBuffer = calloc(1, sizeof(video_metadata_t));
		if (!pBuffHead->pBuffer)
		{
			DEBUG(DEB_LEV_ERR, "err!BuffersMng_List malloc fail!%s,%d\n",__FILE__,__LINE__);
			return OMX_ErrorInsufficientResources;
		}
		memset(pBuffHead->pBuffer, 0, sizeof(video_metadata_t));

		((video_metadata_t*) (pBuffHead->pBuffer))->handle = calloc(1, sizeof(video_handle_t));
		if (((video_metadata_t*) (pBuffHead->pBuffer))->handle == NULL)
		{
			DEBUG(DEB_LEV_ERR, "err!BuffersMng_List malloc fail!%s,%d\n",__FILE__,__LINE__);
			free(pBuffHead->pBuffer);
			pBuffHead->pBuffer = NULL;
			return OMX_ErrorInsufficientResources;
		}
		memset(((video_metadata_t*) (pBuffHead->pBuffer))->handle, 0, sizeof(video_handle_t));

		/*����*/
		input_addr = phy_addr;
		((video_handle_t*) ((video_metadata_t*) (pBuffHead->pBuffer))->handle)->phys_addr = (unsigned long) input_addr;

		DEBUG(DEB_LEV_PARAMS, "AllocateBuffer,bStoreMetaData!input_addr:%p,phy_addr:%p,vir_addr:%p\n",input_addr,phy_addr,(OMX_PTR)vir_addr);
#else
		DEBUG(DEB_LEV_ERR, "err!AllocateBuffer,bStoreMetaData!%s,%d\n",__FILE__,__LINE__);
		return OMX_ErrorInsufficientResources;
#endif
	}
	else
	{
		input_addr = (OMX_PTR) vir_addr;
		pBuffHead->pBuffer = input_addr;

		DEBUG(DEB_LEV_PARAMS, "AllocateBuffer,no bStoreMetaData!input_addr:%p,phy_addr:%p,vir_addr:%p\n",input_addr,phy_addr,(OMX_PTR)vir_addr);
	}

	/*���ҿ�λ*/
	for (i = 0; i < MAX_VCE_MNG_Buffers; i++)
	{
		if (pList[i].IsValid == OMX_FALSE)
		{
			/*���*/
			pList[i].IsValid = OMX_TRUE;
			pList[i].InPutAddr = input_addr;
			pList[i].PhyAddr = phy_addr;
			pList[i].VirAddr = (OMX_PTR) vir_addr;
			pList[i].Ion_Size = 0;
			pBuffersMng_List->Actual_Buffers++;

			isExist = 1;
			break;
		}
	}

	if (isExist == 0)
		DEBUG(DEB_LEV_ERR, "Add_AllocateBuffer,Warning!can not find the buffer\n");

	return OMX_ErrorNone;
}

OMX_ERRORTYPE Add_UseBuffer_BuffersMng(OMX_VCE_Buffers_List*pBuffersMng_List,OMX_BUFFERHEADERTYPE*pBuffHead,int IsStoreMedia,int nSizeBytes,OMX_PTR pBuffer)
{
	OMX_ERRORTYPE eError = OMX_ErrorNone;
	OMX_PTR input_addr = NULL;
	OMX_PTR phy_addr = NULL;
	unsigned long vir_addr = 0;
	OMX_VCE_Buffer_Info* pList = pBuffersMng_List->pList;
	unsigned int isExist = 0;
	unsigned int i;

	//DEBUG(DEB_LEV_PARAMS, "Add_UseBuffer,Actual_Buffers:%d\n",pBuffersMng_List->Actual_Buffers);
	if (pBuffersMng_List->Actual_Buffers >= MAX_VCE_MNG_Buffers)
	{
		DEBUG(DEB_LEV_ERR, "err!BuffersMng_List is full!%s,%d\n",__FILE__,__LINE__);
		return OMX_ErrorInsufficientResources;
	}

	if (IsStoreMedia)
	{
#ifndef enable_gralloc
		/*�����ڴ�*/
		pBuffHead->pBuffer = calloc(1, sizeof(video_metadata_t));
		if (!pBuffHead->pBuffer)
		{
			DEBUG(DEB_LEV_ERR, "err!BuffersMng_List malloc fail!%s,%d\n",__FILE__,__LINE__);
			return OMX_ErrorInsufficientResources;
		}
		memset(pBuffHead->pBuffer, 0, sizeof(video_metadata_t));

		((video_metadata_t*) (pBuffHead->pBuffer))->handle = calloc(1, sizeof(video_handle_t));
		if (((video_metadata_t*) (pBuffHead->pBuffer))->handle == NULL)
		{
			DEBUG(DEB_LEV_ERR, "err!BuffersMng_List malloc fail!%s,%d\n",__FILE__,__LINE__);
			free(pBuffHead->pBuffer);
			pBuffHead->pBuffer = NULL;
			return OMX_ErrorInsufficientResources;
		}
		memset(((video_metadata_t*) (pBuffHead->pBuffer))->handle, 0, sizeof(video_handle_t));
#endif
		/*����ӳ�������ַ*/
		DEBUG(DEB_LEV_PARAMS, "UseBuffer,bStoreMetaData!no input_addr,phy_addr,vir_addr\n");
	}
	else
	{
		input_addr = pBuffer;
		pBuffHead->pBuffer = pBuffer;

		/*���������ڴ�*/
		phy_addr = omx_malloc_phy(nSizeBytes, &vir_addr);
		if (phy_addr == NULL)
		{
			DEBUG(DEB_LEV_ERR, "err!BuffersMng_List malloc fail!%s,%d\n",__FILE__,__LINE__);
			return OMX_ErrorInsufficientResources;
		}

		DEBUG(DEB_LEV_PARAMS, "UseBuffer,no bStoreMetaData!input_addr:%p,phy_addr:%p,vir_addr:%p\n",input_addr,phy_addr,(OMX_PTR)vir_addr);

		/*���ҿ�λ*/
		for (i = 0; i < MAX_VCE_MNG_Buffers; i++)
		{
			if (pList[i].IsValid == OMX_FALSE)
			{
				/*���*/
				pList[i].IsValid = OMX_TRUE;
				pList[i].InPutAddr = input_addr;
				pList[i].PhyAddr = phy_addr;
				pList[i].VirAddr = (OMX_PTR) vir_addr;
				pList[i].Ion_Size = 0;
				pBuffersMng_List->Actual_Buffers++;

				isExist = 1;
				break;
			}
		}

		if (isExist == 0)
			DEBUG(DEB_LEV_ERR, "Add_UseBuffer,Warning!can not find the buffer\n");
	}

	return OMX_ErrorNone;
}

OMX_ERRORTYPE Free_AllocateBuffer_BuffersMng(OMX_VCE_Buffers_List*pBuffersMng_List,OMX_BUFFERHEADERTYPE*pBuffHead,int IsStoreMedia)
{
	OMX_ERRORTYPE eError = OMX_ErrorNone;
	OMX_PTR input_addr = NULL;
	OMX_PTR phy_addr = NULL;
	OMX_PTR vir_addr = NULL;
	OMX_VCE_Buffer_Info* pList = pBuffersMng_List->pList;
	unsigned int isExist = 0;
	unsigned int i;

	DEBUG(DEB_LEV_PARAMS, "Free_AllocateBuffer,Actual_Buffers:%ld\n", pBuffersMng_List->Actual_Buffers);

	if (pBuffersMng_List->Actual_Buffers < 0)
	{
		DEBUG(DEB_LEV_ERR, "err!BuffersMng_List is empty!%s,%d\n",__FILE__,__LINE__);
		return OMX_ErrorInsufficientResources;
	}

	if (IsStoreMedia)
	{
#ifdef enable_gralloc
		eError = VCE_OSAL_GetPhyAddr( (OMX_PTR)((video_metadata_t*)(pBuffHead->pBuffer))->handle,&input_addr);
		if(eError != OMX_ErrorNone)
		{
			DEBUG(DEB_LEV_ERR, "err!VCE_OSAL_GetPhyAddr fail!%s,%d\n",__FILE__,__LINE__);
			return eError;
		}
#else
		input_addr = (OMX_PTR)(((video_handle_t*) ((video_metadata_t*) (pBuffHead->pBuffer))->handle)->phys_addr);
#endif
	}
	else
	{
		input_addr = pBuffHead->pBuffer;
	}

	/*����*/
	for (i = 0; i < MAX_VCE_MNG_Buffers; i++)
	{
		if (pList[i].IsValid == OMX_TRUE)
		{
			if (pList[i].InPutAddr == input_addr)
			{
				/*����*/
				phy_addr = pList[i].PhyAddr;
				vir_addr = pList[i].VirAddr;

				/*���*/
				pList[i].IsValid = OMX_FALSE;
				pList[i].InPutAddr = NULL;
				pList[i].PhyAddr = NULL;
				pList[i].VirAddr = NULL;
				pList[i].Ion_Size = 0;
				pBuffersMng_List->Actual_Buffers--;

				isExist = 1;
				break;
			}
		}
	}

	if (isExist == 0)
		DEBUG(DEB_LEV_ERR, "Free_AllocateBuffer,Warning!can not find the buffer\n");

	if (IsStoreMedia)
	{
#ifndef enable_gralloc
		/*�ͷ��ڴ�*/
		if (((video_metadata_t*) (pBuffHead->pBuffer))->handle)
		{
			free(((video_metadata_t*) (pBuffHead->pBuffer))->handle);
			((video_metadata_t*) (pBuffHead->pBuffer))->handle = NULL;
		}
		if (isExist && pBuffHead->pBuffer)
		{
			free(pBuffHead->pBuffer);
			pBuffHead->pBuffer = NULL;
		}
		DEBUG(DEB_LEV_PARAMS, "Free_AllocateBuffer,bStoreMetaData!input_addr:%p,phy_addr:%p,vir_addr:%p\n",input_addr,phy_addr,vir_addr);
#else
		DEBUG(DEB_LEV_ERR, "err!Free_AllocateBuffer_BuffersMng,bStoreMetaData!%s,%d\n",__FILE__,__LINE__);
#endif
	}
	else
	{
		DEBUG(DEB_LEV_PARAMS, "Free_AllocateBuffer,no bStoreMetaData!input_addr:%p,phy_addr:%p,vir_addr:%p\n",input_addr,phy_addr,vir_addr);
	}

	/*�ͷ�����������ַ*/
	if (isExist && phy_addr)
		omx_free_phy(phy_addr);

	return OMX_ErrorNone;
}

OMX_ERRORTYPE Free_UseBuffer_BuffersMng(OMX_VCE_Buffers_List*pBuffersMng_List,OMX_BUFFERHEADERTYPE*pBuffHead,int IsStoreMedia)
{
	OMX_ERRORTYPE eError = OMX_ErrorNone;
	OMX_PTR input_addr = NULL;
	OMX_PTR phy_addr = NULL;
	OMX_PTR vir_addr = NULL;
	OMX_VCE_Buffer_Info* pList = pBuffersMng_List->pList;
	unsigned int isExist = 0;
	unsigned int i;

	DEBUG(DEB_LEV_PARAMS, "Free_UseBuffer,Actual_Buffers:%ld\n", pBuffersMng_List->Actual_Buffers);

	if (pBuffersMng_List->Actual_Buffers < 0)
	{
		DEBUG(DEB_LEV_ERR, "err!BuffersMng_List is empty!%s,%d\n",__FILE__,__LINE__);
		return OMX_ErrorInsufficientResources;
	}

	if (IsStoreMedia == OMX_TRUE)
	{
#ifndef enable_gralloc
		/*�ͷ��ڴ�*/
		if (((video_metadata_t*) (pBuffHead->pBuffer))->handle)
		{
			free(((video_metadata_t*) (pBuffHead->pBuffer))->handle);
			((video_metadata_t*) (pBuffHead->pBuffer))->handle = NULL;
		}
		if (pBuffHead->pBuffer)
		{
			free(pBuffHead->pBuffer);
			pBuffHead->pBuffer = NULL;
		}
#endif

		/*ע�⣺((video_handle_t*)((video_metadata_t*)(pBuffHead->pBuffer))->handle)->phys_addr �ض�Ϊ NULL*/
		DEBUG(DEB_LEV_PARAMS, "Free_UseBuffer,bStoreMetaData!no input_addr,phy_addr,vir_addr\n");
	}
	else
	{
		input_addr = pBuffHead->pBuffer;

		/*����*/
		for (i = 0; i < MAX_VCE_MNG_Buffers; i++)
		{
			if (pList[i].IsValid == OMX_TRUE)
			{
				if (pList[i].InPutAddr == input_addr)
				{
					/*����*/
					phy_addr = pList[i].PhyAddr;
					vir_addr = pList[i].VirAddr;

					/*���*/
					pList[i].IsValid = OMX_FALSE;
					pList[i].InPutAddr = NULL;
					pList[i].PhyAddr = NULL;
					pList[i].VirAddr = NULL;
					pList[i].Ion_Size = 0;
					pBuffersMng_List->Actual_Buffers--;

					isExist = 1;
					break;
				}
			}
		}

		if (isExist == 0)
			DEBUG(DEB_LEV_ERR, "Free_UseBuffer,Warning!can not find the buffer\n");

		/*�ͷ�����������ַ*/
		if (phy_addr)
			omx_free_phy(phy_addr);

		DEBUG(DEB_LEV_PARAMS, "Free_UseBuffer,no bStoreMetaData!input_addr:%p,phy_addr:%p,vir_addr:%p\n",input_addr,phy_addr,vir_addr);
	}

	return OMX_ErrorNone;
}

OMX_ERRORTYPE Clear_StoreMedia_BuffersMng(OMX_VCE_Buffers_List*pBuffersMng_List)
{
	OMX_ERRORTYPE eError = OMX_ErrorNone;
	OMX_S32 ion_size = 0;
	OMX_PTR vir_addr = NULL;
	OMX_VCE_Buffer_Info* pList = pBuffersMng_List->pList;
	unsigned int isExist = 0;
	unsigned int i;

	DEBUG(DEB_LEV_PARAMS, "Clear_StoreMedia,b4,Actual_Buffers:%ld\n", pBuffersMng_List->Actual_Buffers);

	if (pBuffersMng_List->Actual_Buffers < 0)
	{
		DEBUG(DEB_LEV_ERR, "err!BuffersMng_List is empty!%s,%d\n",__FILE__,__LINE__);
		return OMX_ErrorInsufficientResources;
	}

	//Printf_BuffersMng_List(pBuffersMng_List);

	/*����*/
	for (i = 0; i < MAX_VCE_MNG_Buffers; i++)
	{
		if (pList[i].IsValid == OMX_TRUE)
		{
			/*�ͷ�ӳ�������ַ:��Add_StoreMedia_BuffersMng()��Ӧ*/
			vir_addr = pList[i].VirAddr;
			ion_size = pList[i].Ion_Size;
#ifdef enable_gralloc
			//
#else
			if (vir_addr && ion_size)
			{
				eError = (OMX_ERRORTYPE)omx_munmap_ion_fd(vir_addr, ion_size);
				if(eError != OMX_ErrorNone)
				{
					DEBUG(DEB_LEV_ERR, "err!omx_munmap_ion_fd fail!%s,%d\n",__FILE__,__LINE__);
					return OMX_ErrorUndefined;
				}
			}
#endif

			/*���*/
			pList[i].IsValid = OMX_FALSE;
			pList[i].InPutAddr = NULL;
			pList[i].PhyAddr = NULL;
			pList[i].VirAddr = NULL;
			pList[i].Ion_Size = 0;
			pBuffersMng_List->Actual_Buffers--;
		}
	}

	DEBUG(DEB_LEV_PARAMS, "Clear_StoreMedia,aft,Actual_Buffers:%ld\n", pBuffersMng_List->Actual_Buffers);
	//Printf_BuffersMng_List(pBuffersMng_List);

	return OMX_ErrorNone;
}

static OMX_ERRORTYPE Add_StoreMedia_BuffersMng(OMX_VCE_Buffers_List*pBuffersMng_List, OMX_BUFFERHEADERTYPE*pBuffHead)
{
	OMX_ERRORTYPE eError = OMX_ErrorNone;
	OMX_PTR input_addr = NULL;
	OMX_PTR phy_addr = NULL;
	OMX_PTR vir_addr = NULL;
	OMX_S32 ion_size = 0;
	OMX_VCE_Buffer_Info* pList = pBuffersMng_List->pList;
	unsigned int isExist = 0;
	unsigned int i;

#ifdef enable_gralloc
	eError = VCE_OSAL_GetPhyAddr( (OMX_PTR)((video_metadata_t*)(pBuffHead->pBuffer))->handle,&input_addr);
	if(eError != OMX_ErrorNone)
	{
		DEBUG(DEB_LEV_ERR, "err!VCE_OSAL_GetPhyAddr fail!%s,%d\n",__FILE__,__LINE__);
		return eError;
	}
#else
	input_addr = (OMX_PTR)(((video_handle_t*) ((video_metadata_t*) (pBuffHead->pBuffer))->handle)->phys_addr);
#endif

	/*�����Ƿ����*/
	for (i = 0; i < MAX_VCE_MNG_Buffers; i++)
	{
		if (pList[i].IsValid == OMX_TRUE)
		{
			if (pList[i].InPutAddr == input_addr)
			{
				isExist = 1;
				break;
			}
		}
	}

	//DEBUG(DEB_LEV_PARAMS, "Add_StoreMedia!isExist:%d\n",isExist);

	/*��������*/
	if (isExist == 0)
	{
		DEBUG(DEB_LEV_PARAMS, "Add_StoreMedia,Actual_Buffers:%ld\n", pBuffersMng_List->Actual_Buffers);

		if (pBuffersMng_List->Actual_Buffers >= MAX_VCE_MNG_Buffers)
		{
			DEBUG(DEB_LEV_ERR, "err!BuffersMng_List is full!%s,%d\n",__FILE__,__LINE__);
			return OMX_ErrorInsufficientResources;
		}

		phy_addr = input_addr;

#ifdef enable_gralloc
		int gwidth,gheight,gformat,gsize;
		eError = VCE_OSAL_GetBufInfo( (OMX_PTR)((video_metadata_t*)(pBuffHead->pBuffer))->handle,
				&gwidth,&gheight,&gformat,&gsize);
		if(eError != OMX_ErrorNone)
		{
			DEBUG(DEB_LEV_ERR, "err!VCE_OSAL_GetBufInfo fail!%s,%d\n",__FILE__,__LINE__);
			return eError;
		}
		ion_size = gsize;
		vir_addr = NULL; /*ǿ��*/
#else
		/*����ӳ�������ַ������Clear_StoreMedia_BuffersMng()�ͷ�*/
		ion_size = ((video_handle_t*) ((video_metadata_t *) (pBuffHead->pBuffer))->handle)->size;
		vir_addr = omx_mmap_ion_fd(  ((video_handle_t*)((video_metadata_t *)(pBuffHead->pBuffer))->handle)->ion_share_fd, ion_size);
		if (vir_addr == NULL)
		{
			DEBUG(DEB_LEV_ERR, "err!BuffersMng_List mmap fail!%s,%d\n",__FILE__,__LINE__);
			return OMX_ErrorInsufficientResources;
		}
#endif

		DEBUG(DEB_LEV_PARAMS, "Add_StoreMedia!input_addr:%p,phy_addr:%p,vir_addr:%p\n", input_addr, phy_addr, vir_addr);

		/*���ҿ�λ*/
		for (i = 0; i < MAX_VCE_MNG_Buffers; i++)
		{
			if (pList[i].IsValid == OMX_FALSE)
			{
				/*���*/
				pList[i].IsValid = OMX_TRUE;
				pList[i].InPutAddr = input_addr;
				pList[i].PhyAddr = phy_addr;
				pList[i].VirAddr = vir_addr;
				pList[i].Ion_Size = ion_size;
				pBuffersMng_List->Actual_Buffers++;

				isExist = 1;
				break;
			}
		}

		//Printf_BuffersMng_List(pBuffersMng_List);
		if (isExist == 0)
			DEBUG(DEB_LEV_ERR, "Add_StoreMedia,Warning!can not find the buffer\n");
	}

	DEBUG(DEB_LEV_PARAMS, "SendBuffer,bStoreMetaData!input_addr:%p,phy_addr:%p,vir_addr:%p\n",input_addr,phy_addr,vir_addr);

	return OMX_ErrorNone;
}

OMX_ERRORTYPE SendBuffer_BuffersMng(OMX_VCE_Buffers_List*pBuffersMng_List,OMX_BUFFERHEADERTYPE*pBuffHead,int IsStoreMedia,int Dir)
{
	OMX_ERRORTYPE eError = OMX_ErrorNone;
	OMX_PTR input_addr = NULL;
	OMX_PTR phy_addr = NULL;
	OMX_PTR vir_addr = NULL;
	OMX_VCE_Buffer_Info* pList = pBuffersMng_List->pList;
	unsigned int isExist = 0;
	unsigned int i;

	/*outport��StoreMedia��ʽ������Ҫ����*/
	if (IsStoreMedia == OMX_FALSE && Dir == OMX_DirOutput)
		return OMX_ErrorNone;

	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s for pBuffHead %p\n", __func__, pBuffHead);

	if (pBuffersMng_List->Actual_Buffers < 0)
	{
		DEBUG(DEB_LEV_ERR, "err!BuffersMng_List is empty!%s,%d\n",__FILE__,__LINE__);
		return OMX_ErrorInsufficientResources;
	}
	
	/*����bufferΪ������־֡*/
	if( ((pBuffHead->nFlags & OMX_BUFFERFLAG_EOS) == OMX_BUFFERFLAG_EOS) && (pBuffHead->nFilledLen==0) && (Dir == OMX_DirInput) )
	{
		DEBUG(DEB_LEV_ERR, "SendBuffer,input buffer is eos now!");
		return OMX_ErrorNone;
	}

	/*��IsStoreMedia == OMX_FALSE����Usebufferʱ,pBuffHead->pBuffer != VirAddr*/
	/*Input�Ĳ���Ҫcopy����output����Ҫ*/
	if (IsStoreMedia == OMX_TRUE)
	{
		/*��������:IC5202*/
		eError = Add_StoreMedia_BuffersMng(pBuffersMng_List, pBuffHead);
		if(eError != OMX_ErrorNone)
		{
			DEBUG(DEB_LEV_ERR, "err!Add_StoreMedia_BuffersMng fail!%s,%d\n",__FILE__,__LINE__);
			return eError;
		}
	}
	else /*���buffer��ΪOMX_DirInput*/
	{
		input_addr = pBuffHead->pBuffer;

		/*����*/
		for (i = 0; i < MAX_VCE_MNG_Buffers; i++)
		{
			if (pList[i].IsValid == OMX_TRUE)
			{
				if (pList[i].InPutAddr == input_addr)
				{
					phy_addr = pList[i].PhyAddr;
					vir_addr = pList[i].VirAddr;

					isExist = 1;
					break;
				}
			}
		}

		if (isExist == 0)
			DEBUG(DEB_LEV_ERR, "SendBuffer,Warning!can not find the buffer\n");

		if (vir_addr != input_addr)
		{
			if(pBuffHead->nFilledLen)
			{
				memcpy(vir_addr, input_addr, pBuffHead->nFilledLen);
			}
			else if((pBuffHead->nFlags & OMX_BUFFERFLAG_EOS) != OMX_BUFFERFLAG_EOS)
			{
				DEBUG(DEB_LEV_ERR, "SendBuffer,Warning!nFilledLen:%x,nFlags:%x\n",(int)pBuffHead->nFilledLen,(int)pBuffHead->nFlags);
			}
		}

		DEBUG(DEB_LEV_PARAMS, "SendBuffer,no bStoreMetaData!input_addr:%p,phy_addr:%p,vir_addr:%p\n",input_addr,phy_addr,vir_addr);
	}

	DEBUG(DEB_LEV_FUNCTION_NAME, "out %s for pBuffHead %p\n", __func__, pBuffHead);

	return OMX_ErrorNone;
}

OMX_ERRORTYPE ReturnBuffer_BuffersMng(OMX_VCE_Buffers_List*pBuffersMng_List,OMX_BUFFERHEADERTYPE*pBuffHead,int IsStoreMedia)
{
	OMX_ERRORTYPE eError = OMX_ErrorNone;
	OMX_PTR input_addr = NULL;
	OMX_PTR phy_addr = NULL;
	OMX_PTR vir_addr = NULL;
	OMX_VCE_Buffer_Info* pList = pBuffersMng_List->pList;
	unsigned int isExist = 0;
	unsigned int i;

	if (pBuffersMng_List->Actual_Buffers < 0)
	{
		DEBUG(DEB_LEV_ERR, "err!BuffersMng_List is empty!%s,%d\n",__FILE__,__LINE__);
		return OMX_ErrorInsufficientResources;
	}

	if (IsStoreMedia)
	{
#ifdef enable_gralloc
		eError = VCE_OSAL_GetPhyAddr( (OMX_PTR)((video_metadata_t*)(pBuffHead->pBuffer))->handle,&input_addr);
		if(eError != OMX_ErrorNone)
		{
			DEBUG(DEB_LEV_ERR, "err!VCE_OSAL_GetPhyAddr fail!%s,%d\n",__FILE__,__LINE__);
			return eError;
		}
#else
		input_addr = (OMX_PTR)(((video_handle_t*) ((video_metadata_t*) (pBuffHead->pBuffer))->handle)->phys_addr);
#endif
	}
	else
	{
		input_addr = pBuffHead->pBuffer;
	}

	/*����*/
	for (i = 0; i < MAX_VCE_MNG_Buffers; i++)
	{
		if (pList[i].IsValid == OMX_TRUE)
		{
			if (pList[i].InPutAddr == input_addr)
			{
				phy_addr = pList[i].PhyAddr;
				vir_addr = pList[i].VirAddr;

				isExist = 1;
				break;
			}
		}
	}

	if (isExist == 0)
		DEBUG(DEB_LEV_ERR, "ReturnBuffer,Warning!can not find the buffer\n");

	/*��IsStoreMedia == OMX_FALSE����Usebufferʱ,pBuffHead->pBuffer != VirAddr*/
	/*output�Ĳ���Ҫcopy����Input����Ҫ*/
	if (IsStoreMedia == OMX_FALSE)
	{
		OMX_U32 offset = pBuffHead->nOffset;
		if (vir_addr != input_addr)
		{
			if(pBuffHead->nFilledLen)
				memcpy((OMX_U8*) input_addr + offset, (OMX_U8*) vir_addr + offset, pBuffHead->nFilledLen);
			else if((pBuffHead->nFlags & OMX_BUFFERFLAG_EOS) != OMX_BUFFERFLAG_EOS)
				DEBUG(DEB_LEV_ERR, "ReturnBuffer,Warning!nFilledLen:%x,nFlags:%x\n",(int)pBuffHead->nFilledLen,(int)pBuffHead->nFlags);
		}
		DEBUG(DEB_LEV_PARAMS, "ReturnBuffer,offset:%lu  nFilledLen:%lu\n", offset, pBuffHead->nFilledLen);
		DEBUG(DEB_LEV_PARAMS, "ReturnBuffer,no bStoreMetaData!input_addr:%p,phy_addr:%p,vir_addr:%p\n",input_addr,phy_addr,vir_addr);
	}
	else
	{
		DEBUG(DEB_LEV_PARAMS, "ReturnBuffer,bStoreMetaData!input_addr:%p,phy_addr:%p,vir_addr:%p\n",input_addr,phy_addr,vir_addr);
	}

	return OMX_ErrorNone;
}

OMX_ERRORTYPE Get_UseRingBuffer_BuffersMng(OMX_VCE_Buffers_List*pBuffersMng_List,void* bufferpool,OMX_BUFFERHEADERTYPE*pBuffHead,int ringbuf_size)
{
	OMX_VCE_Buffer_Info* pList = pBuffersMng_List->pList;
	OMX_PTR input_addr = NULL;
	OMX_PTR phy_addr = NULL;
	OMX_PTR vir_addr = NULL;
	unsigned int isExist = 0;
	unsigned int i;

	DEBUG(DEB_LEV_PARAMS, "Get_UseRingBuffer,Actual_Buffers:%ld\n", pBuffersMng_List->Actual_Buffers);
	if (pBuffersMng_List->Actual_Buffers >= MAX_VCE_MNG_Buffers)
	{
		DEBUG(DEB_LEV_ERR, "err!BuffersMng_List is full!%s,%d\n",__FILE__,__LINE__);
		return OMX_ErrorInsufficientResources;
	}

	if (bufferpool == NULL || pBuffHead == NULL)
	{
		DEBUG(DEB_LEV_ERR, "err!bufferpool or pBuffHead is NULL!%s,%d\n",__FILE__,__LINE__);
		return OMX_ErrorInsufficientResources;
	}

	input_addr = get_wbuf(bufferpool, ringbuf_size);
	phy_addr = get_wbuf_phy(bufferpool);
	vir_addr = input_addr;
	if (input_addr == NULL || phy_addr == NULL)
	{
		DEBUG(DEB_LEV_ERR, "err!get_wbuf is NULL!%s,%d\n",__FILE__,__LINE__);
		return OMX_ErrorInsufficientResources;
	}

	pBuffHead->pBuffer = input_addr;
	pBuffHead->nAllocLen = ringbuf_size;
	pBuffHead->nOffset = 0;

	/*���ҿ�λ*/
	for (i = 0; i < MAX_VCE_MNG_Buffers; i++)
	{
		if (pList[i].IsValid == OMX_FALSE)
		{
			/*���*/
			pList[i].IsValid = OMX_TRUE;
			pList[i].InPutAddr = input_addr;
			pList[i].PhyAddr = phy_addr;
			pList[i].VirAddr = vir_addr;
			pList[i].Ion_Size = 0;
			pBuffersMng_List->Actual_Buffers++;

			isExist = 1;
			break;
		}
	}

	if (isExist == 0)
		DEBUG(DEB_LEV_ERR, "Get_UseRingBuffer,Warning!can not find the buffer\n");

	return OMX_ErrorNone;
}

OMX_ERRORTYPE Free_UseRingBuffer_BuffersMng(OMX_VCE_Buffers_List*pBuffersMng_List,void* bufferpool,OMX_BUFFERHEADERTYPE*pBuffHead,int ringbuf_size)
{
	OMX_VCE_Buffer_Info* pList = pBuffersMng_List->pList;
	OMX_PTR input_addr = NULL;
	unsigned int isExist = 0;
	unsigned int i;

	DEBUG(DEB_LEV_PARAMS, "Free_UseRingBuffer,Actual_Buffers:%ld\n", pBuffersMng_List->Actual_Buffers);
	if (pBuffersMng_List->Actual_Buffers < 0)
	{
		DEBUG(DEB_LEV_ERR, "err!BuffersMng_List is empty!%s,%d\n",__FILE__,__LINE__);
		return OMX_ErrorInsufficientResources;
	}

	if (bufferpool == NULL || pBuffHead == NULL)
	{
		DEBUG(DEB_LEV_ERR, "err!bufferpool or pBuffHead is NULL!%s,%d\n",__FILE__,__LINE__);
		return OMX_ErrorInsufficientResources;
	}

	if (pBuffHead->pBuffer != get_poolbase(bufferpool))
	{
		input_addr = pBuffHead->pBuffer;
		free_wbuf(bufferpool, pBuffHead->pBuffer, ringbuf_size);
		pBuffHead->pBuffer = get_poolbase(bufferpool);
		pBuffHead->nAllocLen = 0;
		pBuffHead->nOffset = 0;

		/*����*/
		for (i = 0; i < MAX_VCE_MNG_Buffers; i++)
		{
			if (pList[i].IsValid == OMX_TRUE)
			{
				if (pList[i].InPutAddr == input_addr)
				{
					/*���*/
					pList[i].IsValid = OMX_FALSE;
					pList[i].InPutAddr = NULL;
					pList[i].PhyAddr = NULL;
					pList[i].VirAddr = NULL;
					pList[i].Ion_Size = 0;
					pBuffersMng_List->Actual_Buffers--;

					isExist = 1;
					break;
				}
			}
		}

		if (isExist == 0)
			DEBUG(DEB_LEV_ERR, "Free_UseRingBuffer,Warning!can not find the buffer\n");
	}

	return OMX_ErrorNone;
}

void* Get_PhyAddr_BuffersMng(OMX_VCE_Buffers_List* pBuffersMng_List, OMX_BUFFERHEADERTYPE*pBuffHead, int IsStoreMedia)
{
	OMX_ERRORTYPE eError = OMX_ErrorNone;
	OMX_PTR input_addr = NULL;
	OMX_PTR return_addr = NULL;
	OMX_VCE_Buffer_Info* pList = pBuffersMng_List->pList;
	unsigned int isExist = 0;
	unsigned int i;

	if (IsStoreMedia)
	{
#ifdef enable_gralloc
		eError = VCE_OSAL_GetPhyAddr( (OMX_PTR)((video_metadata_t*)(pBuffHead->pBuffer))->handle,&input_addr);
		if(eError != OMX_ErrorNone)
		{
			DEBUG(DEB_LEV_ERR, "err!VCE_OSAL_GetPhyAddr fail!%s,%d\n",__FILE__,__LINE__);
			return NULL;
		}
#else
		input_addr = (OMX_PTR)(((video_handle_t*) ((video_metadata_t*) (pBuffHead->pBuffer))->handle)->phys_addr);
#endif
	}
	else
	{
		input_addr = pBuffHead->pBuffer;
	}

	/*����*/
	for (i = 0; i < MAX_VCE_MNG_Buffers; i++)
	{
		if (pList[i].IsValid == OMX_TRUE)
		{
			if (pList[i].InPutAddr == input_addr)
			{
				return_addr = pList[i].PhyAddr;

				isExist = 1;
				break;
			}
		}
	}

	if (isExist == 0)
		DEBUG(DEB_LEV_ERR, "Get_PhyAddr,Warning!can not find the buffer\n");

	return return_addr;
}

void* Get_VirAddr_BuffersMng(OMX_VCE_Buffers_List*pBuffersMng_List, OMX_BUFFERHEADERTYPE*pBuffHead, int IsStoreMedia)
{
	OMX_ERRORTYPE eError = OMX_ErrorNone;
	OMX_PTR input_addr = NULL;
	OMX_PTR return_addr = NULL;
	OMX_VCE_Buffer_Info* pList = pBuffersMng_List->pList;
	unsigned int isExist = 0;
	unsigned int i;

	if (IsStoreMedia)
	{
#ifdef enable_gralloc
		eError = VCE_OSAL_GetPhyAddr( (OMX_PTR)((video_metadata_t*)(pBuffHead->pBuffer))->handle,&input_addr);
		if(eError != OMX_ErrorNone)
		{
			DEBUG(DEB_LEV_ERR, "err!VCE_OSAL_GetPhyAddr fail!%s,%d\n",__FILE__,__LINE__);
			return NULL;
		}
#else
		input_addr = (OMX_PTR)(((video_handle_t*) ((video_metadata_t*) (pBuffHead->pBuffer))->handle)->phys_addr);
#endif
	}
	else
	{
		input_addr = pBuffHead->pBuffer;
	}

	/*����*/
	for (i = 0; i < MAX_VCE_MNG_Buffers; i++)
	{
		if (pList[i].IsValid == OMX_TRUE)
		{
			if (pList[i].InPutAddr == input_addr)
			{
				return_addr = pList[i].VirAddr;

				isExist = 1;
				break;
			}
		}
	}

	if (isExist == 0)
		DEBUG(DEB_LEV_ERR, "Get_VirAddr,Warning!can not find the buffer\n");

#ifdef enable_gralloc
	if (IsStoreMedia)
	{
		if (return_addr != NULL)
			DEBUG(DEB_LEV_ERR, "Free_VirAddr,Warning!return_addr is not NULL,unreasonable!\n");

		int gwidth,gheight,gformat,gsize;
		eError = VCE_OSAL_GetBufInfo( (OMX_PTR)((video_metadata_t*)(pBuffHead->pBuffer))->handle,
			&gwidth,&gheight,&gformat,&gsize);
		if(eError != OMX_ErrorNone)
		{
			DEBUG(DEB_LEV_ERR, "err!VCE_OSAL_GetBufInfo fail!%s,%d\n",__FILE__,__LINE__);
			return NULL;
		}

		eError = VCE_OSAL_LockANBHandleWidthUsage( (OMX_PTR)((video_metadata_t*)(pBuffHead->pBuffer))->handle,
			gwidth,gheight,OMX_VCE_GRALLOC_USAGE,&return_addr);
		if(eError != OMX_ErrorNone)
		{
			DEBUG(DEB_LEV_ERR, "err!VCE_OSAL_LockANBHandleWidthUsage fail!%s,%d\n",__FILE__,__LINE__);
			return NULL;
		}
		if (isExist) pList[i].VirAddr = return_addr;
	}
#endif

	return return_addr;
}

#ifdef enable_gralloc
OMX_ERRORTYPE UnLock_VirAddr_BuffersMng(OMX_VCE_Buffers_List*pBuffersMng_List,OMX_BUFFERHEADERTYPE*pBuffHead,int IsStoreMedia)
{
	OMX_ERRORTYPE eError = OMX_ErrorNone;
	OMX_PTR input_addr = NULL;
	OMX_PTR phy_addr = NULL;
	OMX_PTR vir_addr = NULL;
	OMX_VCE_Buffer_Info* pList = pBuffersMng_List->pList;
	unsigned int isExist = 0;
	unsigned int i;

	DEBUG(DEB_LEV_PARAMS, "UnLock_VirAddr,Actual_Buffers:%ld\n", pBuffersMng_List->Actual_Buffers);

	if (pBuffersMng_List->Actual_Buffers < 0)
	{
		DEBUG(DEB_LEV_ERR, "err!BuffersMng_List is empty!%s,%d\n",__FILE__,__LINE__);
		return OMX_ErrorInsufficientResources;
	}

	if (IsStoreMedia == OMX_TRUE)
	{
		eError = VCE_OSAL_GetPhyAddr( (OMX_PTR)((video_metadata_t*)(pBuffHead->pBuffer))->handle,&input_addr);
		if(eError != OMX_ErrorNone)
		{
			DEBUG(DEB_LEV_ERR, "err!VCE_OSAL_GetPhyAddr fail!%s,%d\n",__FILE__,__LINE__);
			return eError;
		}

		/*����*/
		for (i = 0; i < MAX_VCE_MNG_Buffers; i++)
		{
			if (pList[i].IsValid == OMX_TRUE)
			{
				if (pList[i].InPutAddr == input_addr)
				{
					/*����*/
					phy_addr = pList[i].PhyAddr;
					vir_addr = pList[i].VirAddr;

					/*���*/
					pList[i].VirAddr = NULL;
					isExist = 1;
					break;
				}
			}
		}

		if (isExist == 0)
			DEBUG(DEB_LEV_ERR, "UnLock_VirAddr,Warning!can not find the buffer\n");
		
		/*�ͷ�lock�������ַ*/
		if (vir_addr == NULL)
			DEBUG(DEB_LEV_ERR, "UnLock_VirAddr,Warning!vir_addr is NULL\n");
		else
		{
			eError = VCE_OSAL_UnlockANBHandle( (OMX_PTR)(((video_metadata_t*)(pBuffHead->pBuffer))->handle));
			if(eError != OMX_ErrorNone)
			{
				DEBUG(DEB_LEV_ERR, "err!VCE_OSAL_UnlockANBHandle fail!%s,%d\n",__FILE__,__LINE__);
				return eError;
			}
		}

		DEBUG(DEB_LEV_PARAMS, "UnLock_VirAddr,bStoreMetaData!input_addr:%p,phy_addr:%p,vir_addr:%p\n",input_addr,phy_addr,vir_addr);
	}

	return OMX_ErrorNone;
}
#endif