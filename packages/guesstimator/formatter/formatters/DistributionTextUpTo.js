import {regexBasedFormatter, rangeRegex, SeparatorsDistributionUpTo} from './lib'

export const item = {
  formatterName: 'DISTRIBUTION_NORMAL_TEXT_UPTO',
  ...regexBasedFormatter(rangeRegex(SeparatorsDistributionUpTo)),
}
