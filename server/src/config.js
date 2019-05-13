const AUTH0_SECRET = process.env.AUTH0_SECRET;
const JWT_SECRET = process.env.JWT_SECRET;
const JWT_ISSUER = process.env.JWT_ISSUER || 'Foretold';
const JWT_EXPIN = process.env.JWT_EXPIN || '31 days';

if (!AUTH0_SECRET) throw new ReferenceError('AUTH0_SECRET is not defined');
if (!JWT_SECRET) throw new ReferenceError('JWT_SECRET is not defined');
if (!JWT_ISSUER) throw new ReferenceError('JWT_ISSUER is not defined');
if (!JWT_EXPIN) throw new ReferenceError('JWT_EXPIN is not defined');

module.exports = {
  AUTH0_SECRET,
  JWT_SECRET,
  JWT_ISSUER,
  JWT_EXPIN,
};
