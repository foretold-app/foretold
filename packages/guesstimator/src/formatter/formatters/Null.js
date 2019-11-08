import _ from 'lodash';

//TODOFIX ERRORS
export const item = {
  guesstimateType: 'NONE',
  inputType: 'NONE',
  formatterName: 'NULL',
  matches(g) {
    return true;
  },
  format(g) {
    return { guesstimateType: 'NONE' };
  },
  error({ text }) {
    return _.isEmpty(text) ? {} : { type: 1, subType: 2 };
  },
};
