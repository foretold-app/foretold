import { Sampler as DistributionNormalSampler } from './samplers/DistributionNormal.js';
import { Sampler as DistributionLognormalSampler } from './samplers/DistributionLognormal.js';
import { Sampler as DistributionBetaSampler } from './samplers/DistributionBeta.js';
import { Sampler as DistributionPointSampler } from './samplers/DistributionPoint.js';
import { Sampler as DistributionUniformSampler } from './samplers/DistributionUniform.js';
import { Sampler as FunctionSampler } from './samplers/Function.js';
import { Sampler as DataSampler } from './samplers/Data.js';
import { Sampler as NoneSampler } from './samplers/None.js';

export const Funct = {
  referenceName: 'FUNCTION',
  types: ['FUNCTION'],
  displayName: 'Function',
  sampler: FunctionSampler,
  isRangeDistribution: false,
};

export const DistributionNormal = {
  referenceName: 'NORMAL',
  types: ['DISTRIBUTION', 'NORMAL'],
  displayName: 'Normal',
  sampler: DistributionNormalSampler,
  isRangeDistribution: true,
};

export const DistributionPoint = {
  referenceName: 'POINT',
  types: ['DISTRIBUTION', 'POINT'],
  displayName: 'Point',
  sampler: DistributionPointSampler,
  isRangeDistribution: false,
};

export const DistributionLognormal = {
  referenceName: 'LOGNORMAL',
  types: ['DISTRIBUTION', 'LOGNORMAL'],
  displayName: 'LogNormal',
  sampler: DistributionLognormalSampler,
  isRangeDistribution: true,
};

export const DistributionBeta = {
  referenceName: 'BETA',
  types: ['DISTRIBUTION', 'BETA'],
  displayName: 'Beta',
  sampler: DistributionBetaSampler,
  isRangeDistribution: false,
};


export const DistributionUniform = {
  referenceName: 'UNIFORM',
  types: ['DISTRIBUTION', 'UNIFORM'],
  displayName: 'Uniform',
  sampler: DistributionUniformSampler,
  isRangeDistribution: true,
};

export const Data = {
  referenceName: 'DATA',
  types: ['DATA'],
  displayName: 'Data',
  sampler: DataSampler,
  isRangeDistribution: false,
};

// Change to null Guesstimate for sampler
export const None = {
  referenceName: 'NONE',
  types: [],
  displayName: 'NONE',
  sampler: NoneSampler,
  isRangeDistribution: false,
};

const all = [
  Funct,
  DistributionNormal,
  DistributionBeta,
  DistributionPoint,
  DistributionLognormal,
  DistributionUniform,
  Data,
  None
];

export function find(referenceName) {
  const found = all.find(e => e.referenceName === referenceName);
  return found || None;
}

export const samplerTypes = {
  find: referenceName => find(referenceName),
  all,
};
