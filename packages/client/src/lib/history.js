const _wr = function (type) {
  const orig = history[type];
  return function () {
    const rv = orig.apply(this, arguments);
    const e = new Event(type);
    e.arguments = arguments;
    window.dispatchEvent(e);
    return rv;
  };
};

history.pushState = _wr('pushState');
history.replaceState = _wr('replaceState');

function onReplaceState(clb) {
  window.addEventListener('replaceState', function(e) {
    console.debug('replaceState');
    clb();
  });
}

function onPushState(clb) {
  window.addEventListener('pushState', function(e) {
    console.debug('pushState');
    clb();
  });
}

function onPopState(clb) {
  window.addEventListener('popstate', function(e) {
    console.debug('popstate');
    clb();
  });
}

module.exports = {
  onReplaceState,
  onPushState,
  onPopState,
};
