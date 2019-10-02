const { runListeners } = require('./sync/listeners');
const { runJobs } = require('./sync');

runListeners();
runJobs();
