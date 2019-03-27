type meWithTokensAndUserLoading = {
  authTokens: Context__Auth.AuthTokens.t,
  loadingUserData: E.HttpResponse.t(Context__Primary.User.t),
};

type meWithTokensAndUserData = {
  authTokens: Context__Auth.AuthTokens.t,
  userData: Context__Primary.User.t,
};

type me =
  | WithoutTokens
  | WithTokensAndUserLoading(meWithTokensAndUserLoading)
  | WithTokensAndUserData(meWithTokensAndUserData);