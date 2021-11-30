#!/bin/bash

set -x

cd itree

HIQ_SITE_PACKAGE=$(python3 -c 'import sysconfig; print(sysconfig.get_paths()["purelib"])')

mkdir -p build &&
    pushd build &&
    cmake .. -DLOCAL_BUILD=ON &&
    make -j $(nproc) &&
    python -c "import _itree; print(dir(_itree)); print(dir(_itree.Node)); print(dir(_itree.Tree));print(dir(_itree.ForestStats))" &&
    cp _itree*so .. &&
    cp _itree*exe .. &&
    popd

exit 0

if [ "$1" == "local" ]; then
    pip install --editable . --force
else
    python setup.py sdist bdist_wheel
    pip install --force-reinstall dist/hiq-1.0.0-py3-none-any.whl
fi
#sudo cp -f _itree.*.so $HIQ_SITE_PACKAGE/ &&
