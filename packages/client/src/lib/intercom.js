function intercom(name, email) {
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

  var options = {
    app_id: 'gx67sx4f',
    name: name,
    email: email,
    created_at: Math.floor(Date.now() / 1000),
  };

  window.Intercom('boot', options);
  window.Intercom('update');
}

module.exports = { intercom };
