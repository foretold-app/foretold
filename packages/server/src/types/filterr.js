const { measurementCompetitorType } = require('./enums/measurement-competitor-type');
const { measurableValueType } = require('./enums/measurable-value-type');

/**
 * @todo: Refactor this. It's obviously quite hacky.
 * @deprecated
 * @param fields
 * @return {{[p: string]: *}}
 */
function filterr(fields) {
  let newFields = { ...fields };

  if (!!newFields.competitorType) {
    newFields.competitorType = {
      type: measurementCompetitorType,
    };
  }

  if (!!newFields.valueType) {
    newFields.valueType = {
      type: measurableValueType,
    };
  }

  return newFields;
}

module.exports = {
  filterr,
};
