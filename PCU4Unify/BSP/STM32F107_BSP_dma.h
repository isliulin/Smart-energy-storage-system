#ifndef __DMA_H
#define	__DMA_H	   

#ifdef __cplusplus
 extern "C" {
#endif
   

void MYDMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);//配置DMA1_CHx

void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);//使能DMA1_CHx

void NVIC_DMAConfig(void);


#ifdef __cplusplus
}
#endif

#endif
