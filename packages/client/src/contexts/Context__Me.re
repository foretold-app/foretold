type meWithTokensAndUserLoading = {
  loadingUserData: E.HttpResponse.t(Primary.User.t),
};

type meWithTokensAndUserData = {userData: Primary.User.t};

type me =
  | WithoutTokens
  | WithTokensAndUserLoading(meWithTokensAndUserLoading)
  | WithTokensAndUserData(meWithTokensAndUserData);