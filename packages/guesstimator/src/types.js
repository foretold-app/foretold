import {Sampler as DistributionNormalSampler} from './samplers/DistributionNormal'
import {Sampler as DistributionLognormalSampler} from './samplers/DistributionLognormal'
import {Sampler as DistributionBetaSampler} from './samplers/DistributionBeta'
import {Sampler as DistributionPointSampler} from './samplers/DistributionPoint'
import {Sampler as DistributionUniformSampler} from './samplers/DistributionUniform'
import {Sampler as FunctionSampler} from './samplers/Function'
import {Sampler as DataSampler} from './samplers/Data'
import {Sampler as NoneSampler} from './samplers/None'

export const Funct = {
  referenceName: 'FUNCTION',
  types: ['FUNCTION'],
  displayName: 'Function',
  sampler: FunctionSampler,
}

export const DistributionNormal = {
  referenceName: 'NORMAL',
  types: ['DISTRIBUTION', 'NORMAL'],
  displayName: 'Normal',
  sampler: DistributionNormalSampler,
  isRangeDistribution: true,
}

export const DistributionPoint = {
  referenceName: 'POINT',
  types: ['DISTRIBUTION', 'POINT'],
  displayName: 'Point',
  sampler: DistributionPointSampler,
}

export const DistributionLognormal = {
  referenceName: 'LOGNORMAL',
  types: ['DISTRIBUTION', 'LOGNORMAL'],
  displayName: 'LogNormal',
  isRangeDistribution: true,
  sampler: DistributionLognormalSampler,
}

export const DistributionBeta = {
  referenceName: 'BETA',
  types: ['DISTRIBUTION', 'BETA'],
  displayName: 'Beta',
  isRangeDistribution: false,
  sampler: DistributionBetaSampler,
}


export const DistributionUniform = {
  referenceName: 'UNIFORM',
  types: ['DISTRIBUTION', 'UNIFORM'],
  displayName: 'Uniform',
  isRangeDistribution: true,
  sampler: DistributionUniformSampler,
}

export const Data = {
  referenceName: 'DATA',
  types: ['DATA'],
  displayName: 'Data',
  isRangeDistribution: false,
  sampler: DataSampler,
}

// Change to null Guesstimate for sampler
export const None = {
  referenceName: 'NONE',
  types: [],
  displayName: 'NONE',
  sampler: NoneSampler
}

const all = [
  Funct,
  DistributionNormal,
  DistributionBeta,
  DistributionPoint,
  DistributionLognormal,
  DistributionUniform,
  Data,
  None
]

export function find(referenceName) {
  const found = all.find(e => e.referenceName === referenceName)
  return found || None
}

export const samplerTypes = {
  find: referenceName => find(referenceName),
  all
}
