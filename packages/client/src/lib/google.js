window.dataLayer = window.dataLayer || [];

function gtag() {
  dataLayer.push(arguments);
}

if (window.ENV && window.ENV.CLIENT_ENV === "production"){
  console.debug('Google is initialized.');
  gtag('js', new Date());
  gtag('config', 'UA-150341182-1');
}
