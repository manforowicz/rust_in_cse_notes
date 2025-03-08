#!/bin/bash

set -e;
mdbook clean;
mdbook build;
rsync -a ./book/ manfor@attu.cs.washington.edu:/cse/web/homes/manfor/cse_rust_book/;
