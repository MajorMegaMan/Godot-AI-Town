
cd godot-cpp
scons target=debug
cd ..
scons target=debug

mkdir VS
cd VS
cmake .. -DCMAKE_BUILD_TYPE=Debug .
cd ..