#!/bin/sh
VERSION=$(git describe --tags --always --abbrev=7 --long)
echo "#ifndef VERSION_H" > ../Core/Inc/version.h
echo "#define VERSION_H" >> ../Core/Inc/version.h
echo "#define GIT_VERSION \"$VERSION\"" >> ../Core/Inc/version.h
echo "#endif // VERSION_H" >> ../Core/Inc/version.h