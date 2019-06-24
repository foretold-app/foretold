open Context.Routing;
open Foretold__GraphQL;
open Rationale.Function.Infix;
open PageConfig;

module EntityShow' = {
  let toEl = EntityShow.make |> LoggedInPage.justPageParams;
};

module BotCreate' = {
  let toEl = BotCreate.make |> LoggedInPage.noParams;
};

module BotEdit' = {
  let toEl = BotEdit.make |> LoggedInPage.justPageParams;
};

module AgentIndex' = {
  let toEl = AgentIndex.make |> LoggedInPage.noParams;
};

module Profile' = {
  let toEl = Profile.make |> LoggedInPage.justLoggedInUser;
};

module ChannelIndex' = {
  let toEl = ChannelIndex.make |> LoggedInPage.justLoggedInUser;
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

module Auth0Redirect' = {
  let toEl = loggedInUser =>
    Auth0Redirect.make(~loggedInUser) |> E.React.makeToEl;
};