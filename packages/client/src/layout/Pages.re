open PageConfig;

module EntityShow' = {
  let toEl = EntityShow.make |> LoggedInPage.justPageParams;
};

module BotCreate' = {
  let toEl = BotCreate.make |> LoggedInPage.justLoggedInUser;
};

module BotEdit' = {
  let toEl = BotEdit.make |> LoggedInPage.pageParamsAndUser;
};

module AgentIndex' = {
  let toEl = AgentIndex.make |> LoggedInPage.noParams;
};

module Profile' = {
  let toEl = Profile.make |> LoggedInPage.justLoggedInUser;
};

module Preferences' = {
  let toEl = Preferences.make |> LoggedInPage.justLoggedInUser;
};

module ChannelIndex' = {
  let toEl = ChannelIndex.Jsx2.make |> LoggedInPage.justLoggedInUser;
};

module ChannelNew' = {
  let toEl = ChannelNew.make |> LoggedInPage.noParams;
};

module MeasurableEdit' = {
  let toEl = MeasurableEdit.make |> LoggedInPage.justPageParams;
};

module EntityIndex' = {
  let toEl = EntityIndex.make |> LoggedInPage.noParams;
};

module Redirecting' = {
  let toEl = loggedInUser =>
    Redirecting.make(~loggedInUser) |> E.React.makeToEl;
};