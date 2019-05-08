console.log('PM2 Env', process.env);

module.exports = {
  apps: [
    {
      name: "server",
      script: "./server/src/index.js",
      env: process.env,
      env_production: process.env,
    },
    {
      name: "bots",
      script: "./bots/index.js",
      env: process.env,
      env_production: process.env,
    }
  ]
};
