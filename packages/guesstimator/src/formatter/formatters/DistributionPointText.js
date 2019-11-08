import { POINT_REGEX, regexBasedFormatter } from '../lib';

export const item = {
  formatterName: 'DISTRIBUTION_POINT_TEXT',
  ...regexBasedFormatter(POINT_REGEX, () => 'POINT', () => {
  }),
};

