echo "Workspace Fixer: begin"

rm -rf ./packages/bs-distplus/node_modules/bs-moment
rm -rf ./packages/bs-distplus/node_modules/@glennsl/bs-jest
rm -rf ./packages/client/node_modules/bs-moment
rm -rf ./packages/server/node_modules/bs-platform
rm -rf ./node_modules/bs-ken/node_modules/@glennsl/bs-jest

echo "Workspace Fixer: OK"
