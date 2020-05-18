type generic = {
  item: string,
  description: string,
};

type measurable = {
  item: string,
  description: string,
  measurableId: string,
};

type measurableB = {
  item: string,
  description: string,
  measurableId: string,
  labelSubject: option(string),
  labelProperty: option(string),
  labelCustom: option(string),
  labelStartAtDate: option(MomentRe.Moment.t),
  labelEndAtDate: option(MomentRe.Moment.t),
  labelConditionals: option(array(string)),
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

type series = {
  item: string,
  description: string,
  seriesId: string,
};

type body =
  | Generic(generic)
  | Measurable(measurable)
  | MeasurableB(measurableB)
  | Series(series)
  | Measurement(measurement)
  | Notebook(notebook)
  | Channel(channel)
  | JoinedMember(joinedMember)
  | Empty;

module Generic = {
  type t = generic;
  let make = (~item, ~description, ()): t => {item, description};

  let fromJson = m => {
    make(~item=m##item, ~description=m##description, ());
  };
};

module Measurable = {
  type t = measurable;

  let make = (~item, ~description, ~measurableId, ()): t => {
    item,
    description,
    measurableId,
  };

  let fromJson = m => {
    make(
      ~item=m##item,
      ~description=m##description,
      ~measurableId=m##measurableId,
      (),
    );
  };
};

module MeasurableB = {
  type t = measurableB;

  let make =
      (
        ~item,
        ~description,
        ~measurableId,
        ~labelSubject=None,
        ~labelProperty=None,
        ~labelCustom=None,
        ~labelStartAtDate=None,
        ~labelEndAtDate=None,
        ~labelConditionals=None,
        (),
      )
      : t => {
    item,
    description,
    measurableId,

    labelSubject,
    labelProperty,
    labelCustom,
    labelStartAtDate,
    labelEndAtDate,
    labelConditionals,
  };

  let fromJson = m => {
    make(
      ~item=m##item,
      ~description=m##description,
      ~measurableId=m##measurableId,
      ~labelSubject=m##labelSubject,
      ~labelProperty=m##labelProperty,
      ~labelCustom=m##labelCustom,
      ~labelStartAtDate=m##labelStartAtDate,
      ~labelEndAtDate=m##labelEndAtDate,
      ~labelConditionals=m##labelConditionals,
      (),
    );
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

  let fromJson = m => {
    make(
      ~item=m##item,
      ~description=m##description,
      ~measurableId=m##measurableId,
      ~measurementId=m##measurementId,
      (),
    );
  };
};

module Notebook = {
  type t = notebook;

  let make = (~item, ~description, ~notebookId, ()): t => {
    item,
    description,
    notebookId,
  };

  let fromJson = m => {
    make(
      ~item=m##item,
      ~description=m##description,
      ~notebookId=m##notebookId,
      (),
    );
  };
};

module JoinedMember = {
  type t = joinedMember;

  let make = (~item, ~description, ()): t => {item, description};

  let fromJson = m => {
    make(~item=m##item, ~description=m##description, ());
  };
};

module Channel = {
  type t = channel;

  let make = (~item, ~description, ()): t => {item, description};

  let fromJson = m => {
    make(~item=m##item, ~description=m##description, ());
  };
};

module Series = {
  type t = series;

  let make = (~item, ~description, ~seriesId, ()): t => {
    item,
    description,
    seriesId,
  };

  let fromJson = m => {
    make(
      ~item=m##item,
      ~description=m##description,
      ~seriesId=m##seriesId,
      (),
    );
  };
};

let toBody = m => {
  switch (
    m##generic,
    m##measurable,
    m##measurement,
    m##notebook,
    m##channel,
    m##joinedMember,
    m##series,
    m##measurableB,
  ) {
  | (Some(m), _, _, _, _, _, _, _) => Generic(Generic.fromJson(m))
  | (_, Some(m), _, _, _, _, _, _) => Measurable(Measurable.fromJson(m))
  | (_, _, Some(m), _, _, _, _, _) => Measurement(Measurement.fromJson(m))
  | (_, _, _, Some(m), _, _, _, _) => Notebook(Notebook.fromJson(m))
  | (_, _, _, _, Some(m), _, _, _) => Channel(Channel.fromJson(m))
  | (_, _, _, _, _, Some(m), _, _) => JoinedMember(JoinedMember.fromJson(m))
  | (_, _, _, _, _, _, Some(m), _) => Series(Series.fromJson(m))
  | (_, _, _, _, _, _, _, Some(m)) => MeasurableB(MeasurableB.fromJson(m))
  | _ => Empty
  };
};