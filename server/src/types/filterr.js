const { competitor } = require('./competitor');
const { valueType } = require('./value-type');

/**
 * @deprecated
 * @param fields
 * @return {{[p: string]: *}}
 */
function filterr(fields) {
  let newFields = { ...fields };
  if (!!newFields.competitorType) {
    newFields.competitorType = { type: competitor }
  }
  if (!!newFields.valueType) {
    newFields.valueType = { type: valueType }
  }
  return newFields;
}

module.exports = {
  filterr,
};
