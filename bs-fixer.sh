rm -rf ./packages/client/node_modules/@foretold/cdf
rm -rf ./packages/client/node_modules/@foretold/guesstimator
rm -rf ./packages/client/node_modules/@foretold/components
rm -rf ./packages/client/node_modules/@foretold/bs-distplus

rsync -av --progress ./packages/cdf ./packages/client/node_modules/@foretold --exclude node_modules
rsync -av --progress ./packages/guesstimator ./packages/client/node_modules/@foretold --exclude node_modules
rsync -av --progress ./packages/components ./packages/client/node_modules/@foretold --exclude node_modules
rsync -av --progress ./packages/bs-distplus ./packages/client/node_modules/@foretold --exclude node_modules

rm -rf ./packages/client/node_modules/bs-ant-design-alt/node_modules/bs-moment
