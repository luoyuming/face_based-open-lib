mkdir build
cd build
rm -rf *

FACE_PATH=face_image
mkdir ${FACE_PATH}
cp ../*.jpg  ./
cp ../*.jpg  ./${FACE_PATH}/
cp ../test.py ./

cmake ..
make
python3 test.py