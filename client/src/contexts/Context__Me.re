type meWithTokensAndUserLoading = {
  authTokens: Context__Auth.AuthTokens.t,
  loadingUserData: ApolloUtils.QResponse.tri(Context__Primary.User.t),
};

type meWithTokensAndUserData = {
  authTokens: Context__Auth.AuthTokens.t,
  userData: Context__Primary.User.t,
};

type me =
  | WithoutTokens
  | WithTokensAndUserLoading(meWithTokensAndUserLoading)
  | WithTokensAndUserData(meWithTokensAndUserData);