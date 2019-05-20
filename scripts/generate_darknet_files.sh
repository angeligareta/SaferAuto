# Update submodules 
git submodule update --init --recursive

# Post-installation steps for cuda
export PATH=/usr/local/cuda-10.0/bin${PATH:+:${PATH}}
export LD_LIBRARY_PATH=/usr/local/cuda-10.0/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}

# Activate GPU acceleration and OPENCV
cd darknet
sed -i 's/GPU=.*/GPU=1/' Makefile
sed -i 's/CUDNN=.*/CUDNN=1/' Makefile
sed -i 's/CUDNN_HALF=.*/CUDNN_HALF=1/' Makefile
sed -i 's/OPENCV=.*/OPENCV=1/' Makefile
sed -i 's/LIBSO=.*/LIBSO=1/' Makefile
make

# Save important compiled file
cp libdarknet.so ../lib