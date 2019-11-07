import _ from 'lodash';

//FIXME
export const item = {
  formatterName: 'FUNCTION',
  matches({ text }) {
    return !!text && text.startsWith('=');
  },
  error({ text }) {
    return !_.isEmpty(text.slice(1)) ? {} : { type: 1, subType: 2 };
  },
  format({ text }) {
    return { guesstimateType: 'FUNCTION', text: text.slice(1) };
  },
};
