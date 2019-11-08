import { rangeRegex, regexBasedFormatter } from '../lib';

export const item = {
  formatterName: 'DISTRIBUTION_NORMAL_TEXT_UPTO',
  ...regexBasedFormatter(rangeRegex(/,\s?/, /\[/, /\]/)),
};
