const { competitorType } = require('./competitor-type');
const { measurableValueType } = require('./measurable-value-type');

/**
 * @deprecated
 * @param fields
 * @return {{[p: string]: *}}
 */
function filterr(fields) {
  let newFields = { ...fields };
  // TODO: Refactor this. It's obviously quite hacky.
  if (!!newFields.competitorType) {
    newFields.competitorType = { type: competitorType };
  }
  if (!!newFields.valueType) {
    newFields.valueType = { type: measurableValueType };
  }
  return newFields;
}

module.exports = {
  filterr,
};
