/**
 * @param {string} intercomAppId
 * @param {string} name
 * @param {string} email
 * @param {Moment} createdAt
 */
function intercom(intercomAppId, name, email, createdAt) {
  (function () {
    var w = window;
    var ic = w.Intercom;
    if (typeof ic === 'function') {
      ic('reattach_activator');
      ic('update', w.intercomSettings);
    } else {
      var d = document;
      var i = function () {
        i.c(arguments);
      };
      i.q = [];
      i.c = function (args) {
        i.q.push(args);
      };
      w.Intercom = i;
      var l = function () {
        var s = d.createElement('script');
        s.type = 'text/javascript';
        s.async = true;
        s.src = 'https://widget.intercom.io/widget/gx67sx4f';
        var x = d.getElementsByTagName('script')[0];
        x.parentNode.insertBefore(s, x);
      };
      l();
    }
  })();

  var now = Math.floor(Date.now() / 1000);
  var signupAt = !!createdAt && 'unix' in createdAt
    ? createdAt.unix()
    : now;

  var options = {
    app_id: intercomAppId,
    name: name,
    email: email,
    created_at: signupAt,
  };

  window.Intercom('boot', options);
  window.Intercom('update');

  console.debug("Intercom init", options);
}

module.exports = { intercom };
