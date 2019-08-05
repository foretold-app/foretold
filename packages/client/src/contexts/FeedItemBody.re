type common = {
  item: string,
  description: string,
};
type t = {common: option(common)};

let make = (~common, ()): t => {common: common};

module Common = {
  type t = common;
  let make = (~item, ~description, ()): t => {item, description};
};