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

module Generic = {
  type t = generic;
  let make = (~item, ~description, ()): t => {item, description};

  let toCommon = m => {
    switch (m) {
    | Some(generic) =>
      make(~item=generic##item, ~description=generic##description, ())
      |> E.O.some
    | _ => None
    };
  };
};

module Measurable = {
  type t = measurable;

  let make = (~item, ~description, ~measurableId, ()): t => {
    item,
    description,
    measurableId,
  };

  let toMeasurable = m => {
    switch (m) {
    | Some(measurable) =>
      make(
        ~item=measurable##item,
        ~description=measurable##description,
        ~measurableId=measurable##measurableId,
        (),
      )
      |> E.O.some
    | _ => None
    };
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

  let toMeasurement = m => {
    switch (m) {
    | Some(measurement) =>
      make(
        ~item=measurement##item,
        ~description=measurement##description,
        ~measurableId=measurement##measurableId,
        ~measurementId=measurement##measurementId,
        (),
      )
      |> E.O.some
    | _ => None
    };
  };
};

module Notebook = {
  type t = notebook;

  let make = (~item, ~description, ~notebookId, ()): t => {
    item,
    description,
    notebookId,
  };

  let toNotebook = m => {
    switch (m) {
    | Some(notebook) =>
      make(
        ~item=notebook##item,
        ~description=notebook##description,
        ~notebookId=notebook##notebookId,
        (),
      )
      |> E.O.some
    | _ => None
    };
  };
};

module JoinedMember = {
  type t = joinedMember;

  let make = (~item, ~description, ()): t => {item, description};

  let toJoinedMember = m => {
    switch (m) {
    | Some(joinedMember) =>
      make(
        ~item=joinedMember##item,
        ~description=joinedMember##description,
        (),
      )
      |> E.O.some
    | _ => None
    };
  };
};

module Channel = {
  type t = channel;

  let make = (~item, ~description, ()): t => {item, description};

  let toChannel2 = m => {
    switch (m) {
    | Some(channel2) =>
      make(~item=channel2##item, ~description=channel2##description, ())
      |> E.O.some
    | _ => None
    };
  };
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

let toBody = m => {
  make(
    ~generic=Generic.toCommon(m##generic),
    ~measurable=Measurable.toMeasurable(m##measurable),
    ~measurement=Measurement.toMeasurement(m##measurement),
    ~notebook=Notebook.toNotebook(m##notebook),
    ~channel=Channel.toChannel2(m##channel),
    ~joinedMember=JoinedMember.toJoinedMember(m##joinedMember),
    (),
  );
};