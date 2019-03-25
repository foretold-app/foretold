type meWithTokensAndUserLoading = {
  authTokens: Context__Auth.AuthTokens.t,
  loadingUserData: ApolloUtils.QResponse.tri(DataModel.User.t),
};

type meWithTokensAndUserData = {
  authTokens: Context__Auth.AuthTokens.t,
  userData: DataModel.User.t,
};

type me =
  | WithoutTokens
  | WithTokensAndUserLoading(meWithTokensAndUserLoading)
  | WithTokensAndUserData(meWithTokensAndUserData);