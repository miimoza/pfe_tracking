#!/bin/sh

echo "#####################"
echo "# TARBALL GENERATOR #"
echo "#####################"

tarball="william.chow_antoine.coulon_vincent.payet-labonne_dorian.vinai.tar.gz"

echo "cleaning..."
echo "rm -f $tarball"
rm -f "$tarball"

echo "${tarball}:"
tar -czvf "$tarball" "src" "CMakeLists.txt" "README" "build.sh"
