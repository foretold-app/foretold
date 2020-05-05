echo "Workspace Fixer"

rm -rf ./packages/bs-distplus/node_modules/bs-moment
rm -rf ./packages/bs-distplus/node_modules/@glennsl/bs-jest
rm -rf ./packages/client/node_modules/bs-moment
rm -rf ./packages/server/node_modules/bs-platform
rm -rf ./node_modules/bs-ken/node_modules/@glennsl/bs-jest

#mkdir -p ./packages/components/node_modules/react-textarea-autosize/node_modules/@babel/runtime
#rsync -av --progress ./node_modules/@babel/runtime/ ./packages/components/node_modules/react-textarea-autosize/node_modules/@babel/runtime/

echo "OK"
