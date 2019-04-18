type meWithTokensAndUserLoading = {
  authTokens: Context__Auth.Auth0Tokens.t,
  loadingUserData: E.HttpResponse.t(Context__Primary.User.t),
};

type meWithTokensAndUserData = {
  authTokens: Context__Auth.Auth0Tokens.t,
  userData: Context__Primary.User.t,
};

type me =
  | WithoutTokens
  | WithTokensAndUserLoading(meWithTokensAndUserLoading)
  | WithTokensAndUserData(meWithTokensAndUserData);