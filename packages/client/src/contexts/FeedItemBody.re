type generic = {
  item: string,
  description: string,
};

type measurable = {
  item: string,
  description: string,
  measurableId: string,
};

type measurement = {
  item: string,
  description: string,
  measurableId: string,
  measurementId: string,
};

type notebook = {
  item: string,
  description: string,
  notebookId: string,
};

type channel = {
  item: string,
  description: string,
};

type joinedMember = {
  item: string,
  description: string,
};

type t = {
  generic: option(generic),
  measurable: option(measurable),
  measurement: option(measurement),
  notebook: option(notebook),
  channel: option(channel),
  joinedMember: option(joinedMember),
};

let make =
    (
      ~generic,
      ~measurable,
      ~channel,
      ~measurement,
      ~notebook,
      ~joinedMember,
      (),
    )
    : t => {
  generic,
  measurable,
  measurement,
  channel,
  joinedMember,
  notebook,
};

module Generic = {
  type t = generic;
  let make = (~item, ~description, ()): t => {item, description};
};

module Measurable = {
  type t = measurable;
  let make = (~item, ~description, ~measurableId, ()): t => {
    item,
    description,
    measurableId,
  };
};

module Measurement = {
  type t = measurement;
  let make = (~item, ~description, ~measurableId, ~measurementId, ()): t => {
    item,
    description,
    measurableId,
    measurementId,
  };
};

module Notebook = {
  type t = notebook;
  let make = (~item, ~description, ~notebookId, ()): t => {
    item,
    description,
    notebookId,
  };
};

module JoinedMember = {
  type t = joinedMember;
  let make = (~item, ~description, ()): t => {item, description};
};

module Channel = {
  type t = channel;
  let make = (~item, ~description, ()): t => {item, description};
};