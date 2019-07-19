type meWithTokensAndUserLoading = {
  loadingUserData: HttpResponse.t(Primary.User.t),
};

type meWithTokensAndUserData = {userData: Primary.User.t};

type t =
  | WithoutTokens
  | WithTokensAndUserLoading(meWithTokensAndUserLoading)
  | WithTokensAndUserData(meWithTokensAndUserData);