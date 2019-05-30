CUDA_PATH	=	/usr/local/apps/cuda/cuda-9.2
CUDA_BIN_PATH	=	$(CUDA_PATH)/bin
CUDA_NVCC	=	$(CUDA_BIN_PATH)/nvcc
all: arrayMul montecarlo
arrayMul:	arrayMul.cu
		$(CUDA_NVCC) -o arrayMul  arrayMul.cu
montecarlo:	montecarlo.cu
		$(CUDA_NVCC) -o montecarlo montecarlo.cu
clean:
		rm arrayMul montecarlo
