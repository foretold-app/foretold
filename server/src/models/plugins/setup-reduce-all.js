/**
 * Copied from
 * https://github.com/sequelize/sequelize/issues/2454
 * @param Sequelize
 */
function setupReduceAll(Sequelize) {
  // findAll in chunks and reduce to single value.
  // This is done in a memory efficient way by taking x rows each time.
  // Where x is defined by options.limit.
  //
  // options :      options passed to findAll. options.limit defines how many rows
  //                should be reduced per iteration.
  // action :       callback that is called for each iteration.
  //                gets parameter data and previous reduce value.
  //                The callback should return new reduce value
  //                or a Promise that returns a reduced value.
  // initialValue : Initial value of for the reduction.
  //
  // Returns Promise with reduced value
  //
  // Example:
  //    var initialSum = 0;
  //    Model
  //        .reduceAll({
  //            limit: 100
  //        }, function (data, sum) {
  //            return data.reduce(function (row, currentSum) {
  //                return currentSum + row.number;
  //            }, sum);
  //        }, initialSum)
  //        .then(function (totalSum) {
  //            console.info('Total sum is: ', totalSum);
  //        });

  Sequelize.Model.prototype.reduceAll = function reduceAll(options, action, initialValue) {
    let self = this;
    let foundRecords = -1;
    let reducedValue = initialValue;

    options = options || {};
    options.limit = options.limit || 1000;
    options.offset = 0;

    function reduceNextChunk() {
      if (foundRecords === 0) {
        // No more records found in table
        // return the final reduced value.
        return reducedValue;
      } else {
        return self
          .findAll(options)
          .tap(function (data) {
            foundRecords = data.length;
            // Increase the offset for next interation.
            options.offset += options.limit;
          })
          .then(function (data) {
            // Call the callback with data rows and last reduced value.
            return action(data, reducedValue);
          })
          .then(function (newReducedValue) {
            // Update reduced value
            reducedValue = newReducedValue;
          })
          // Find next chunk
          .then(reduceNextChunk);
      }
    }

    return reduceNextChunk();
  };
}

module.exports = {
  setupReduceAll,
};
