function wrapper(type) {
  const orig = history[type];
  return function (state, title, path) {
    const rv = orig.apply(this, [state, title, path]);
    const e = new Event(type);
    e.state = state;
    window.dispatchEvent(e);
    return rv;
  };
}

history.pushState = wrapper('pushState');
history.replaceState = wrapper('replaceState');

function onReplaceState(clb) {
  window.addEventListener('replaceState', function (event) {
    clb(event);
  });
}

function onPushState(clb) {
  window.addEventListener('pushState', function (event) {
    clb(event);
  });
}

function onPopState(clb) {
  window.addEventListener('popstate', function (event) {
    clb(event);
  });
}

function pushState(state, title, path) {
  history.pushState(state, title, path);
}

function replaceState(state, title, path) {
  history.replaceState(state, title, path);
}

function getHistoryState() {
  return window.history.state;
}

module.exports = {
  onReplaceState,
  onPushState,
  onPopState,
  pushState,
  replaceState,
  getHistoryState,
};
