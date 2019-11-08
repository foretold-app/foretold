import { rangeRegex, regexBasedFormatter, DISTRIBUTION_RANGE_REGEX } from '../lib';

export const item = {
  formatterName: 'DISTRIBUTION_NORMAL_TEXT_UPTO',
  ...regexBasedFormatter(rangeRegex(DISTRIBUTION_RANGE_REGEX)),
};
