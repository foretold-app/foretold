# prediction-analysis
unlink ./packages/prediction-analysis/node_modules/@foretold/cdf
cp -r ./packages/cdf/ ./packages/prediction-analysis/node_modules/@foretold/cdf

# components
unlink ./packages/components/node_modules/@foretold/cdf
unlink ./packages/components/node_modules/@foretold/guesstimator
cp -r ./packages/cdf/ ./packages/components/node_modules/@foretold/cdf
cp -r ./packages/guesstimator/ ./packages/components/node_modules/@foretold/guesstimator

# distplus
unlink ./packages/distplus/node_modules/@foretold/guesstimator
cp -r ./packages/guesstimator/ ./packages/distplus/node_modules/@foretold/guesstimator

# client
unlink ./packages/client/node_modules/@foretold/bs-distplus
unlink ./packages/client/node_modules/@foretold/components
cp -r ./packages/distplus/ ./packages/client/node_modules/@foretold/bs-distplus
cp -r ./packages/components/ ./packages/client/node_modules/@foretold/components

# server
unlink ./packages/client/node_modules/@foretold/cdf
unlink ./packages/client/node_modules/@foretold/prediction-analysis
cp -r ./packages/cdf/ ./packages/client/node_modules/@foretold/cdf
cp -r ./packages/prediction-analysis/ ./packages/client/node_modules/@foretold/prediction-analysis
