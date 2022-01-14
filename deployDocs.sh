#!/bin/bash
rm -rf doxygen.log html/ ../.libzed_docs
make docs
git clone -b gh-pages git@github.com:ZacharyWesterman/libzed.git ../.libzed_docs
rm -rf ../.libzed_docs/html
mv html doxygen.log ../.libzed_docs/
cd ../.libzed_docs
git add --all
git status
git commit -m "Updated documentation."
git push
cd ..
rm -rf .libzed_docs
