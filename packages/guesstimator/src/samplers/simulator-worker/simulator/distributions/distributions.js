import jstat from 'jstat'
import Sampling from 'discrete-sampling'

const bernoulli = p => Sampling.Bernoulli(p).draw()
const binomial = (n, p) => Sampling.Binomial(n,p).draw()
const poisson = (lambda) => Sampling.Poisson(lambda).draw()
const negBinomial = (r, p) => Sampling.NegBinomial(r, p).draw()

function triangular(min, max, mode = (min + max)/2) {
  const u = Math.random()
  if (u < (mode-min)/(max-min)) {
    return min + Math.sqrt(u*(max-min)*(mode-min))
  } else {
    return max - Math.sqrt((1-u)*(max-min)*(max-mode))
  }
}

// Source:
// http://www.mhnederlof.nl/doubletriangular.html
function doubleTriangular(min, max, mode = (min + max)/2) {
  const u = Math.random()
  if (u <= 0.5) {
    return min + (mode - min)*Math.sqrt(2*u)
  } else {
    return max - (max - mode)*Math.sqrt(2*(1-u))
  }
}

// Source:
// https://en.wikipedia.org/wiki/Beta_distribution#Transformations
function PERT(min, max, mode = (min + max)/2, lambda = 4) {
  const width = max - min
  const a = 1 + lambda * ((mode - min)/width)
  const b = 1 + lambda * ((max - mode)/width)
  const p = jstat.beta.sample(a, b)
  return min + p*width
}

export const Distributions = {
  beta: jstat.beta.sample,
  centralF: jstat.centralF.sample,
  cauchy: jstat.cauchy.sample,
  chisquare: jstat.chisquare.sample,
  exponential: jstat.exponential.sample,
  invgamma: jstat.invgamma.sample,
  lognormal: jstat.lognormal.sample,
  logn: jstat.lognormal.sample,
  normal: jstat.normal.sample,
  n: jstat.normal.sample,
  studentt: jstat.studentt.sample,
  weibull: jstat.weibull.sample,
  uniform: jstat.uniform.sample,
  gamma: jstat.gamma.sample,
  triangular,
  doubleTriangular,
  PERT,
  bernoulli: bernoulli,
  if: bernoulli,
  test: bernoulli,
  binomial: binomial,
  poisson: poisson,
  negBinomial: negBinomial
}
