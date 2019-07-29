type meWithTokensAndUserLoading = {
  loadingUserData: HttpResponse.t(Types.user),
};

type meWithTokensAndUserData = {userData: Types.user};

type t =
  | WithoutTokens
  | WithTokensAndUserLoading(meWithTokensAndUserLoading)
  | WithTokensAndUserData(meWithTokensAndUserData);