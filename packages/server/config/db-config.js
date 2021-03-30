module.exports = {
  development: {
    username: process.env.DB_USERNAME,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE,
    host: process.env.DB_HOST,
    use_env_variable: process.env.DB_USE_ENV_VARIABLE || undefined,
    dialect: "postgres",
    logging: false
  },
  production: {
    username: process.env.DB_USERNAME,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE,
    host: process.env.DB_HOST,
    use_env_variable: process.env.DB_USE_ENV_VARIABLE || "DATABASE_URL",
    dialect: "postgres",
    logging: false,
    dialectOptions: {
      ssl: true
    }
  },
  test: {
    username: process.env.DB_USERNAME,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE,
    host: process.env.DB_HOST,
    use_env_variable: process.env.DB_USE_ENV_VARIABLE || undefined,
    dialect: "postgres",
    logging: false
  }
};
