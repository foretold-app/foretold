type common = {
  item: string,
  description: string,
};

type measurable = {
  item: string,
  description: string,
  measurableId: string,
};

type t = {
  common: option(common),
  measurable: option(measurable),
};

let make = (~common, ~measurable, ()): t => {common, measurable};

module Common = {
  type t = common;
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