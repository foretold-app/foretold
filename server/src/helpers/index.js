
/**
 * @param {string} str
 * @return {string}
 */
function capitalizeFirstLetter(str) {
  return str.charAt(0).toUpperCase() + str.slice(1);
}

module.exports = {
  capitalizeFirstLetter,
};
