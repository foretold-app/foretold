// @todo: remove this

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

module Preferences' = {
  let toEl = Preferences.make |> LoggedInPage.justLoggedInUser;
};

module ChannelNew' = {
  let toEl = ChannelNew.make |> LoggedInPage.noParams;
};

module MeasurableEdit' = {
  let toEl = MeasurableEdit.make |> LoggedInPage.pageParamsAndUser;
};

module EntityIndex' = {
  let toEl = EntityIndex.make |> LoggedInPage.noParams;
};