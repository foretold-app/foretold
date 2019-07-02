type meWithTokensAndUserLoading = {
  authTokens: Context__Auth.Auth0Tokens.t,
  loadingUserData: E.HttpResponse.t(Primary.User.t),
};

type meWithTokensAndUserData = {
  authTokens: Context__Auth.Auth0Tokens.t,
  userData: Primary.User.t,
};

type me =
  | WithoutTokens
  | WithTokensAndUserLoading(meWithTokensAndUserLoading)
  | WithTokensAndUserData(meWithTokensAndUserData);